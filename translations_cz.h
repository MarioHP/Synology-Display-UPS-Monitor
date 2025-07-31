#ifndef TRANSLATIONS_CZ_H
#define TRANSLATIONS_CZ_H

#include <map>
#include <Arduino.h>
// čeština
std::map<String, String> paramTranslations_cz = {
  { "battery.charge", "Nabití baterie (%)" },
  { "battery.charge.low", "Nízký stav baterie (%)" },
  { "battery.charge.warning", "Varování úrovně baterie (%)" },
  { "battery.mfr.date", "Výrobce baterie / datum" },
  { "battery.runtime", "Zbývající čas" },
  { "battery.runtime.low", "Minimální runtime (s)" },
  { "battery.type", "Typ baterie" },
  { "battery.voltage", "Napětí baterie (V)" },
  { "battery.voltage.nominal", "Nominální napětí baterie (V)" },
  { "device.mfr", "Výrobce zařízení" },
  { "device.model", "Model zařízení" },
  { "device.serial", "Sériové číslo" },
  { "device.type", "Typ zařízení" },
  { "driver.name", "Název ovladače" },
  { "driver.parameter.pollfreq", "Frekvence dotazování (s)" },
  { "driver.parameter.pollinterval", "Interval dotazování (s)" },
  { "driver.parameter.port", "Port ovladače" },
  { "driver.parameter.synchronous", "Synchronní režim" },
  { "driver.version", "Verze ovladače" },
  { "driver.version.data", "Verze dat" },
  { "driver.version.internal", "Interní verze" },
  { "input.transfer.high", "Max vstupní napětí (V)" },
  { "input.transfer.low", "Min vstupní napětí (V)" },
  { "input.voltage", "Vstupní napětí (V)" },
  { "input.voltage.nominal", "Nominální vstupní napětí (V)" },
  { "output.voltage", "Výstupní napětí (V)" },
  { "ups.beeper.status", "Zvukový alarm" },
  { "ups.delay.shutdown", "Zpoždění vypnutí (s)" },
  { "ups.delay.start", "Zpoždění startu (s)" },
  { "ups.load", "Zatížení (%)" },
  { "ups.mfr", "Výrobce UPS" },
  { "ups.model", "Model UPS" },
  { "ups.productid", "Produkt ID" },
  { "ups.realpower.nominal", "Reálný výkon (W)" },
  { "ups.serial", "Sériové číslo UPS" },
  { "ups.status", "Stav UPS" },
  { "ups.test.result", "Výsledek testu" },
  { "ups.timer.shutdown", "Časovač vypnutí (s)" },
  { "ups.timer.start", "Časovač spuštění (s)" },
  { "ups.vendorid", "ID výrobce" },
  // Html
  { "page_title", "UPS monitor" },
  { "uptime_label", "Čas od spuštění" },
  { "refresh", "Obnova stránky" },
  { "firmware.update",  "Aktualizace firmwaru" }, 
  // Hlavni tabulka
  { "table_param", "Parametr" },
  { "table_value", "Hodnota" },
  { "button.table", "Tabulka" },
  { "button.graph", "Graf" },
  { "button.system", "Systém" },
  // RSSI tabulka
  { "rssi.value", "Hodnota RSSI" },
  { "rssi.signal_strength", "Síla signálu WiFi" },
  { "rssi.excellent", "Vynikající" },
  { "rssi.very_good", "Velmi dobrý" },
  { "rssi.good", "Docela dobrý" },
  { "rssi.fair", "Dobrý" },
  { "rssi.poor", "Špatný" },
  { "rssi.very_poor", "Slabý (nepoužitelný)" }
};

#endif
