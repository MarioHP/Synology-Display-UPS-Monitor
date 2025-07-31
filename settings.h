#ifndef SETTINGS_H
#define SETTINGS_H

// WiFi přihlašovací údaje
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// Statické IP nastavení sítě
const IPAddress local_IP(,,,);
const IPAddress gateway(,,,);
const IPAddress subnet(255,255,255,0);
const IPAddress primaryDNS(8, 8, 8, 8);   // Google DNS
const IPAddress secondaryDNS(8, 8, 4, 4); // Google DNS

// UPS server nastavení
const char* upsHost = "..."; // IP adresa NASu / UPS serveru
const uint16_t upsPort = 3493;
const char* upsName = "ups";

// Výchozí jazyk
#define DEFAULT_LANGUAGE "cz"
// #define DEFAULT_LANGUAGE "en"

// Interval obnovy webu v milisekundách (např. 60000 = 60 sekund)
const unsigned long WEBPAGE_RELOAD_INTERVAL_MS = 60000;

// Spořič obrazovky (v milisekundách)
const unsigned long SCREENSAVER_DELAY_MS = 30000;  // 30 sekund

// Jas displeje (rozsah 0–255)
#define BRIGHTNESS_OFF     0
#define BRIGHTNESS_LOW     64    // 25 %
#define BRIGHTNESS_MEDIUM  127   // 50 %
#define BRIGHTNESS_HIGH    191   // 75 %
#define BRIGHTNESS_MAX     255   // 100 %

const int BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW;  // Nastav jas spořiče

#endif