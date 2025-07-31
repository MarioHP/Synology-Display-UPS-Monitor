#ifndef HTML_H
#define HTML_H

#include <Arduino.h>
#include <map>
#include "settings.h"

String generateHtml(const std::map<String, String>& upsData, const std::map<String, String>& translations, const String& lang = String(DEFAULT_LANGUAGE)) {
  unsigned long totalSeconds = millis() / 1000;
  unsigned long hours = totalSeconds / 3600;
  unsigned long minutes = (totalSeconds % 3600) / 60;
  unsigned long seconds = totalSeconds % 60;

  // Přepočet battery.runtime na minuty, sekundy
  String batteryRuntimeStr = "";
  if (upsData.count("battery.runtime")) {
    unsigned long runtimeSec = upsData.at("battery.runtime").toInt();
    unsigned long rtMinutes = runtimeSec / 60;
    unsigned long rtSeconds = runtimeSec % 60;
    batteryRuntimeStr = String(rtMinutes) + " min " + String(rtSeconds) + " s";
  }

  String html;

  // Začátek HTML a hlavička, jazyk z parametru lang
  html += "<!DOCTYPE html><html lang='" + lang + "'><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>" + translations.at("page_title") + "</title>";
  html += "<meta http-equiv='refresh' content='" + String(WEBPAGE_RELOAD_INTERVAL_MS / 1000) + "'>";

  // Styl (beze změny)
  html += R"rawliteral(
  <style>
    html {
      overflow-y: scroll;
    }

    body {
      font-family: 'Roboto', sans-serif;
      max-width: 600px;
      text-align: center;
      background: #f5f5f5;
      margin: 0 auto;
    }

    .headline {
      font-weight: 600;
      letter-spacing: 1px;
      font-size: 26px;
      color: #009688cc;
      border: 1px solid #b2dfdb;
      background-color: #e0f7fa;
      padding: 10px 12px;
      display: block;
      margin: 20px auto 0 auto;
      box-shadow: 2px 2px 6px rgba(0, 150, 136, 0.1);
      border-radius: 4px;
      text-align: center;
    }

    .statusTable {
      width: 100%;
      max-width: 600px;
      margin: 30px auto;
      border-collapse: separate;
      border-spacing: 0;
      border-radius: 6px;
      overflow: hidden;
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.04);
      background-color: #fcfcfd;
      border: 1px solid #e0e4ea;
      table-layout: fixed;
    }

    .statusTable th,
    .statusTable td {
      padding: 12px 16px;
      text-align: left;
      border-bottom: 1px solid #eceff4;
      color: #5e6b7a;
    }

    .statusTable th {
      background-color: #f2f5fa;
      font-weight: 600;
      color: #445566;
      text-transform: uppercase;
      letter-spacing: 0.04em;
      user-select: none;
    }

    .statusTable tbody tr:nth-child(even) {
      background-color: #f8f9fb;
    }

    .statusTable tbody tr:hover {
      background-color: #f0f3f8;
      transition: background-color 0.2s ease;
    }

    .statusTable td:nth-child(2) {
      font-weight: bold;
      color: black;
    }

    p {
      color: #999;
      font-size: 0.9em;
    }

.link-button {
  display: inline-block;
  color: #666;
  text-decoration: none;
  font-weight: 500;
  font-size: 14px;
}


    @media (max-width: 480px) {

  	.statusTable { font-size: 0.9rem; width: 95%; }
  	.statusTable th, td { padding: 10px; }
    .headline { width: 95%; font-size: 1.7rem; padding: 8px 12px; box-sizing: border-box; }
    p { font-size: 0.9rem; line-height: 14px; margin: 16px auto; }

    }
  </style>
  )rawliteral";

  // Nadpis stránky
  html += "<div class='headline'>" + translations.at("page_title") + "</div>";

  // Tabulka
  html += "<table class='statusTable'><thead><tr><th>";
  html += translations.at("table_param");
  html += "</th><th>";
  html += translations.at("table_value");
  html += "</th></tr></thead><tbody>";

  // Jen vybrané položky
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

  for (const String& key : displayKeys) {
    if (upsData.count(key)) {
      String label = translations.count(key) ? translations.at(key) : key;
      if (key == "battery.runtime") {
        html += "<tr><td>" + label + "</td><td>" + batteryRuntimeStr + "</td></tr>";
      } else {
        html += "<tr><td>" + label + "</td><td>" + upsData.at(key) + "</td></tr>";
      }
    }
  }

  // Uptime
  html += "</tbody></table><p>";
  html += "<p>" + translations.at("uptime_label") + ": " + String(hours) + " h " + String(minutes) + " min</p>";
  html += "<p>WiFi: " + String(WiFi.RSSI()) + " dBm</p>";
  html += "<p>" + translations.at("refresh") + ": " + String(WEBPAGE_RELOAD_INTERVAL_MS / 1000) + " s</p>";
  html += "<p><a href='/update' class='link-button'>" + translations.at("firmware.update") + " (OTA)</a></p>";
  html += "</body></html>";

  return html;
}

#endif
