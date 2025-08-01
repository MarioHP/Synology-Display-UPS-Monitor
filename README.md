[🇬🇧 English](README.ENG.md)

# Návod pro Synology NAS

## 1. Instalace Pythonu

- Centrum balíčků → Python 3.9 → Nainstaluj

## 2. Povolení SSH

- Ovládací panel → Terminál a SNMP → ☑️ Povolit službu SSH

## 3. Aktivace složky uživatele

- Ovládací panel → Uživatelé a skupiny → Rozšířené → ☑️ Povolit službu Složka uživatele

## 4. Vytvoření sdílené složky „docker“

- Ovládací panel → Sdílená složka → Vytvořit složku s názvem `docker`

**Zkopíruj do ní následující soubory z UPS Docker:**

- `Dockerfile`
- `ups_to_ttgo.py`
- `docker-compose.yml`


## 5. PuTTY – nastavení

**Povolení vkládání zkopírovaných příkazů:**
- `Window → Selection → Action of mouse buttons: Windows (Right-click pastes)`


**SSH připojení k NAS:**
- `Session → Host Name`: IP adresa Synology
- `Port`: 22  
- `Connection Type`: SSH  
- `Saved Sessions`: Libovolný název → klikni na **Save**


## 6. Zjištění portu TTGO T-Display

**Připoj se k Synology přes PuTTY Session (jméno a heslo) a získej identifikaci zařízení:**
```bash
ls /dev/tty*
```

> mel by se ukázat nějaký připojený port např. /dev/ttyACM0

**Postup vytvoření fixního názvu připojeného portu:**
```bash
sudo udevadm info -a -n /dev/ttyACM0
```

**Hledej například:**
> - ATTRS{idVendor}=="1a86"
> - ATTRS{idProduct}=="55d4"
> - ATTRS{product}=="USB Single Serial"

**Vytvoř udev pravidlo**
```bash
sudo mkdir -p /etc/udev/rules.d
```
**Uprav podle nalezených parametrů:**
```bash
echo 'SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="55d4", SYMLINK+="ups-serial"' | sudo tee /etc/udev/rules.d/99-ups-to-serial.rules
```

**Aktivuj nové pravidlo:**
```bash
sudo udevadm control --reload-rules
```
```bash
sudo udevadm trigger
```

**Ověř funkčnost:**
```bash
ls -l /dev/ups-serial
```

> lrwxrwxrwx 1 root root 7 Jul 31 09:55 /dev/ups-serial -> ttyACM0

## 7. Sestavení Docker obrazu

**Přejdi do adresáře s Dockerfile:** 
```bash
cd /volume1/docker/ups-to-serial/
```
**Sestav a spusť image:**
```bash
sudo docker-compose up --build -d
```

## 8. Test
**Ověř pomocí logu, že kontejner běží a vypisuje odesílaná data**
```bash
sudo docker logs -f ups-to-serial
```
> Odesláno: {"battery.charge": "100", "battery.runtime": "8830", "battery.voltage": "13.8", "device.model": "UT850EG", "input.voltage": "233.0", "output.voltage": "234.0", "battery.mfr.date": "CPS", "battery.type": "PbAcid"}
