#ifndef TRANSLATIONS_EN_H
#define TRANSLATIONS_EN_H

#include <map>
#include <Arduino.h>
// english
std::map<String, String> paramTranslations_en = {
  { "battery.charge", "Battery Charge (%)" },
  { "battery.charge.low", "Low Battery Charge (%)" },
  { "battery.charge.warning", "Battery Charge Warning (%)" },
  { "battery.mfr.date", "Battery Manufacturer / Date" },
  { "battery.runtime", "Remaining Runtime (s)" },
  { "battery.runtime.low", "Minimum Runtime (s)" },
  { "battery.type", "Battery Type" },
  { "battery.voltage", "Battery Voltage (V)" },
  { "battery.voltage.nominal", "Nominal Battery Voltage (V)" },
  { "device.mfr", "Device Manufacturer" },
  { "device.model", "Device Model" },
  { "device.serial", "Serial Number" },
  { "device.type", "Device Type" },
  { "driver.name", "Driver Name" },
  { "driver.parameter.pollfreq", "Polling Frequency (s)" },
  { "driver.parameter.pollinterval", "Polling Interval (s)" },
  { "driver.parameter.port", "Driver Port" },
  { "driver.parameter.synchronous", "Synchronous Mode" },
  { "driver.version", "Driver Version" },
  { "driver.version.data", "Data Version" },
  { "driver.version.internal", "Internal Version" },
  { "input.transfer.high", "Max Input Voltage (V)" },
  { "input.transfer.low", "Min Input Voltage (V)" },
  { "input.voltage", "Input Voltage (V)" },
  { "input.voltage.nominal", "Nominal Input Voltage (V)" },
  { "output.voltage", "Output Voltage (V)" },
  { "ups.beeper.status", "Audible Alarm" },
  { "ups.delay.shutdown", "Shutdown Delay (s)" },
  { "ups.delay.start", "Start Delay (s)" },
  { "ups.load", "Load (%)" },
  { "ups.mfr", "UPS Manufacturer" },
  { "ups.model", "UPS Model" },
  { "ups.productid", "Product ID" },
  { "ups.realpower.nominal", "Real Power (W)" },
  { "ups.serial", "UPS Serial Number" },
  { "ups.status", "UPS Status" },
  { "ups.test.result", "Test Result" },
  { "ups.timer.shutdown", "Shutdown Timer (s)" },
  { "ups.timer.start", "Start Timer (s)" },
  { "ups.vendorid", "Vendor ID" },
  // Html
  { "page_title", "UPS Status" },
  { "uptime_label", "Uptime" },
  { "refresh", "Page refresh" },
  { "firmware.update",  "Firmware Update" },
  // Main table
  { "table_param", "Parameter" },
  { "table_value", "Value" },
  { "button.table", "Table" },
  { "button.graph", "Graph" },
  { "button.system", "System" },
  // RSSI table
  { "rssi.value", "RSSI Value" },
  { "rssi.signal_strength", "WiFi Signal Strength" },
  { "rssi.excellent", "Excellent" },
  { "rssi.very_good", "Very Good" },
  { "rssi.good", "Good" },
  { "rssi.fair", "Fair" },
  { "rssi.poor", "Poor" },
  { "rssi.very_poor", "Very Poor (Unusable)" }
};

#endif
