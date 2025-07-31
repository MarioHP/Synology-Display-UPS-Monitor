#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "settings.h"
#include "html.h"
#include "translations_cz.h"
#include "translations_en.h"
#include <ArduinoJson.h>
#include <driver/ledc.h>
#include <ElegantOTA.h>

#include "FontNotoSansBold20.h"

#define BUTTON_NEXT_PIN 0  // next
#define BUTTON_BACK_PIN 35 // back

#define BACKLIGHT_PIN 4 // pin podsviceni
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

#define TFT_MONITORGREY  0xCE79   /* 204, 204, 204 */ 

#define SMALL_FONT FontNotoSansBold20

WiFiClient client;
AsyncWebServer server(80);
std::map<String, String> upsData;
std::map<String, String>* currentTranslations;

TFT_eSPI tft = TFT_eSPI(135, 240);

int currentPage = 0;
unsigned long lastButtonPress = 0;
const unsigned long debounceTime = 300;

// Data UPS
unsigned long lastReadySend = 0;
const unsigned long READY_INTERVAL_MS = 1000; // posílat READY každou 1 vteřinu

// Spořič
unsigned long lastActivity = 0; 
bool isScreensaverOn = false;

bool readySent = false;
bool receivedAnyData = false;

std::vector<String> displayKeys = {
  "battery.charge",
  "battery.runtime",
  "battery.voltage",
  "device.model",
  "input.voltage",
  "output.voltage",
  "battery.mfr.date",
  "battery.type"     
};

// Buffer pro příjem ze sériové linky
String serialBuffer = "";

enum Language { LANG_CZ, LANG_EN };
Language currentLanguage;

String getLabel(const String& key) {
  if (currentLanguage == LANG_CZ) {
    if      (key == "battery.charge")   return "Nabiti baterie";
    else if (key == "battery.runtime")  return "Zbyvajici cas";
    else if (key == "battery.voltage")  return "Napeti baterie";
    else if (key == "device.model")     return "Model zarizeni";
    else if (key == "input.voltage")    return "Vstupni napeti";
    else if (key == "output.voltage")   return "Vystupni napeti";
    else if (key == "battery.mfr.date") return "Vyrobce baterie";
    else if (key == "battery.type")     return "Typ baterie";
  } else {
    if      (key == "battery.charge")   return "Battery charge";
    else if (key == "battery.runtime")  return "Runtime";
    else if (key == "battery.voltage")  return "Battery voltage";
    else if (key == "device.model")     return "Device model";
    else if (key == "input.voltage")    return "Input voltage";
    else if (key == "output.voltage")   return "Output voltage";
    else if (key == "battery.mfr.date") return "Battery Manufacturer";
    else if (key == "battery.type")     return "Battery Type";    
  }
  return key; // fallback na klic
}

String getUnit(const String& key) {
  if      (key == "battery.charge")    return "%";
  else if (key == "battery.runtime")   return "";  // už je převedeno na "min s"
  else if (key == "battery.voltage")   return "V";
  else if (key == "input.voltage")     return "V";
  else if (key == "output.voltage")    return "V";
  else                                 return ""; // default = žádná jednotka
}


void displayCurrentPage() {
  tft.fillScreen(TFT_BLACK);

  int startIndex = currentPage * 2;

  for (int i = 0; i < 2; i++) {
    int idx = startIndex + i;
    if (idx >= displayKeys.size()) break;

    String key = displayKeys[idx];
    String label = getLabel(key);
    String value = "N/A";

  if (upsData.count(key)) {
    value = upsData[key];

    if (key == "battery.runtime") {
      unsigned long runtimeSec = value.toInt();
      unsigned long rtMinutes = runtimeSec / 60;
      unsigned long rtSeconds = runtimeSec % 60;
      value = String(rtMinutes) + " min " + String(rtSeconds) + " s";
    } else {
      String unit = getUnit(key);
      value += " " + getUnit(key);  // přidání jednotky
    }
  }

    int y = 20 + i * 65; 

    uint16_t color = TFT_WHITE;
    if      (key == "battery.charge")   color = TFT_GREEN;
    else if (key == "battery.runtime")  color = TFT_ORANGE;
    else if (key == "battery.voltage")  color = TFT_YELLOW;
    else if (key == "device.model")     color = TFT_CYAN;
    else if (key == "input.voltage")    color = TFT_BLUE;
    else if (key == "output.voltage")   color = TFT_MAGENTA;
    else if (key == "battery.mfr.date") color = TFT_DARKGREY;
    else if (key == "battery.type")     color = TFT_DARKGREY;

    // Label   
    tft.setTextColor(color, TFT_BLACK);
    tft.loadFont(SMALL_FONT);
    tft.drawString(label + ":", 120, y);
    // Value
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(value, 120, y + 25);
  }
}


// Funkce na zpracování jedné řádky JSON ze sériové linky
void processJsonLine(const String& line) {
  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, line);
  if (!err) {
    upsData.clear();
    for (JsonPair kv : doc.as<JsonObject>()) {
      upsData[kv.key().c_str()] = kv.value().as<String>();
    }
    receivedAnyData = true; // přidáno
    displayCurrentPage();
  } else {
    Serial.println("Chyba při čtení JSON: " + String(err.c_str()));
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // počkat na ustálení linky
  pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  pinMode(BACKLIGHT_PIN, OUTPUT);

  // Inicializace displeje
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Jazyk podle nastavení
  if (strcmp(DEFAULT_LANGUAGE, "en") == 0) {
    currentTranslations = &paramTranslations_en;
    currentLanguage = LANG_EN;
  } else {
    currentTranslations = &paramTranslations_cz;
    currentLanguage = LANG_CZ;
  }

  // Připojení WiFi
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Chyba při nastavování statické IP!");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi připojeno, IP: " + WiFi.localIP().toString());
  // Na displej po připojení
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);  
  tft.loadFont(SMALL_FONT);
  tft.setTextColor(TFT_MONITORGREY);
  tft.drawString("UPS MONITOR", 120, 50); 
  tft.drawString("IP: " + WiFi.localIP().toString(), 120, 80);
  delay(3000);

  // HTTP server a OTA
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      String lang = String(DEFAULT_LANGUAGE);
      const std::map<String, String>* translations = nullptr;

      if (lang == "en") translations = &paramTranslations_en;
      else translations = &paramTranslations_cz;

      String html = generateHtml(upsData, *translations, lang);
      request->send(200, "text/html", html);
  });

  ElegantOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server spuštěn");

  displayCurrentPage();

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);

  if (currentLanguage == LANG_CZ) {
    tft.drawString("Cekam na data...", 120, 60);
  } else {
    tft.drawString("Waiting for data...", 120, 60);
  }

  lastActivity = millis();  // Zahájit měření nečinnosti

}

void checkButtons() {
  int totalPages = (displayKeys.size() + 1) / 2;

  if (digitalRead(BUTTON_NEXT_PIN) == LOW && (millis() - lastButtonPress) > debounceTime) {
    lastButtonPress = millis();
    lastActivity = millis();

    if (isScreensaverOn) {
      analogWrite(BACKLIGHT_PIN, BRIGHTNESS_MAX);  // rozsvítit displej
      isScreensaverOn = false;
      displayCurrentPage();
      return;
    }

    currentPage = (currentPage + 1) % totalPages;
    displayCurrentPage();

    // Rozsvítit jas při stisku tlačítka
    analogWrite(BACKLIGHT_PIN, BRIGHTNESS_MAX);
    isScreensaverOn = false;
  }

  if (digitalRead(BUTTON_BACK_PIN) == LOW && (millis() - lastButtonPress) > debounceTime) {
    lastButtonPress = millis();
    lastActivity = millis();

    if (isScreensaverOn) {
      analogWrite(BACKLIGHT_PIN, BRIGHTNESS_MAX);
      isScreensaverOn = false;
      displayCurrentPage();
      return;
    }

    currentPage = (currentPage - 1 + totalPages) % totalPages;
    displayCurrentPage();

    // Rozsvítit jas při stisku tlačítka
    analogWrite(BACKLIGHT_PIN, BRIGHTNESS_MAX);
    isScreensaverOn = false;
  }
}


void loop() {
  // Posílání "READY" dokud nejsou přijata data
  if (upsData.empty() && (millis() - lastReadySend > READY_INTERVAL_MS)) {
    Serial.println("READY");
    lastReadySend = millis();
  }

  // Příjem dat ze sériové linky bez blokování - vždycky číst!
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processJsonLine(serialBuffer);
      serialBuffer = "";
      // Tady nemenit jas!
      lastActivity = millis();  // reset času nečinnosti
    } else {
      serialBuffer += c;
    }
  }

  checkButtons();

  // Spořič - snížit jas, pokud je nečinnost delší než limit
  if (!isScreensaverOn && (millis() - lastActivity > SCREENSAVER_DELAY_MS)) {
    analogWrite(BACKLIGHT_PIN, BACKLIGHT_BRIGHTNESS); // snížený jas
    isScreensaverOn = true;
  }

  ElegantOTA.loop();
}
