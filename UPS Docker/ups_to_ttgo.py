import time
import serial
import subprocess
import json

SERIAL_PORT = "/dev/ups-serial"
UPS_NAME = "ups"
SLEEP_SECONDS = 60 # čtení jednou za minutu

UPS_FIELDS = [
    "battery.charge",
    "battery.runtime",
    "battery.voltage",
    "device.model",
    "input.voltage",
    "output.voltage",
    "battery.mfr.date",
    "battery.type"         
]

def get_upsc_values():
    result = {}
    for field in UPS_FIELDS:
        try:
            output = subprocess.check_output(["upsc", f"{UPS_NAME}@localhost", field], stderr=subprocess.DEVNULL)
            result[field] = output.decode("utf-8").strip()
        except subprocess.CalledProcessError:
            result[field] = "N/A"
    return result

def main():
    ser = None
    while True:
        if ser is None or not ser.is_open:
            try:
                ser = serial.Serial(SERIAL_PORT, 115200, timeout=1)
                print(f"Sériový port {SERIAL_PORT} otevřen")
            except Exception as e:
                print(f"Nelze otevřít sériový port: {e}, zkusím znovu za 5 sekund")
                time.sleep(5)
                continue

        values = get_upsc_values()
        msg = json.dumps(values) + "\n"
        try:
            ser.write(msg.encode("utf-8"))
            print("Odesláno:", msg.strip())
        except Exception as e:
            print(f"Chyba při zápisu na sériový port: {e}, zavírám port a budu čekat na znovu připojení")
            ser.close()
            ser = None
        time.sleep(SLEEP_SECONDS)

if __name__ == "__main__":
    main()

