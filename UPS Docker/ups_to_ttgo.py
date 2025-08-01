import time
import serial
import subprocess
import json

SERIAL_PORT = "/dev/ups-serial"  
BAUDRATE = 115200
UPS_NAME = "ups"
READ_TIMEOUT = 1  # sekundy pro čekání na řádek
RETRY_DELAY = 5   # při chybě v otevření portu
SLEEP_BETWEEN_REQUESTS = 60  # max interval mezi dvěma odesláními (fail-safe)

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

def send_ups_data(ser):
    values = get_upsc_values()
    msg = json.dumps(values) + "\n"
    try:
        ser.write(msg.encode("utf-8"))
        print("Odesláno:", msg.strip())
    except Exception as e:
        print(f"️ Chyba při zápisu na port: {e}")
        raise e

def main():
    ser = None
    last_sent = 0

    while True:
        if ser is None or not ser.is_open:
            try:
                ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=READ_TIMEOUT)
                print(f" Sériový port {SERIAL_PORT} otevřen")
                send_ups_data(ser)  # po otevření hned první dávka
                last_sent = time.time()
            except Exception as e:
                print(f" Nelze otevřít sériový port: {e}, zkusím znovu za {RETRY_DELAY}s")
                time.sleep(RETRY_DELAY)
                continue

        try:
            if ser.in_waiting:
                line = ser.readline().decode("utf-8").strip()
                print(" Přijato:", line)
                if line == "READY":
                    send_ups_data(ser)
                    last_sent = time.time()
        except Exception as e:
            print(f"️ Chyba při čtení ze sériového portu: {e}, zavírám...")
            ser.close()
            ser = None
            continue

        # fail-safe – odeslat alespoň 1× za 60 sekund i bez READY
        if time.time() - last_sent > SLEEP_BETWEEN_REQUESTS:
            try:
                send_ups_data(ser)
                last_sent = time.time()
            except:
                ser.close()
                ser = None
        time.sleep(0.1)

if __name__ == "__main__":
    main()

