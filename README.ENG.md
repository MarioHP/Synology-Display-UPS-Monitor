[Read in Czech](README.md)  

# Guide for Synology NAS

<img width="400" alt="image" src="https://github.com/user-attachments/assets/803339a9-ad88-4c91-ab43-5e1570b2866d" />

3D print:

https://www.printables.com/model/1371767-synology-cyberpower-ups-monitor

https://makerworld.com/en/models/1659413-synology-cyberpower-ups-monitor


## 1. Installing packages

- Package Center → Python 3.9 → Install
- Package Center → Container Manager → Install

## 2. Enabling SSH

- Control Panel → Terminal & SNMP → ☑️ Enable SSH service

## 3. Enabling User Home Folder

- Control Panel → Users and Groups → Advanced → ☑️ Enable User Home Service

## 4. Creating Shared Folder “docker”

- Control Panel → Shared Folder → Create a folder named `docker`

**Copy the following files from UPS Docker into it:**

- `Dockerfile`
- `ups_to_ttgo.py`
- `docker-compose.yml`

## 5. PuTTY – Configuration

**Enable pasting of copied commands:**
- `Window → Selection → Action of mouse buttons: Windows (Right-click pastes)`

**SSH connection to NAS:**
- `Session → Host Name`: Synology IP address  
- `Port`: 22  
- `Connection Type`: SSH  
- `Saved Sessions`: Any name → click **Save**

## 6. Identifying TTGO T-Display Port

**Connect to Synology via PuTTY Session (username and password) and get device identification:**
```bash
ls /dev/tty*
```

> You should see some connected port, e.g. /dev/ttyACM0

**Steps to create a fixed name for the connected port:**
```bash
sudo udevadm info -a -n /dev/ttyACM0
```

**Look for example:**
> - ATTRS{idVendor}=="1a86"  
> - ATTRS{idProduct}=="55d4"  
> - ATTRS{product}=="USB Single Serial"

**Create udev rule**
```bash
sudo mkdir -p /etc/udev/rules.d
```
**Edit according to the detected parameters:**
```bash
echo 'SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="55d4", SYMLINK+="ups-serial"' | sudo tee /etc/udev/rules.d/99-ups-to-serial.rules
```

**Activate the new rule:**
```bash
sudo udevadm control --reload-rules
```
```bash
sudo udevadm trigger
```

**Verify functionality:**
```bash
ls -l /dev/ups-serial
```

> lrwxrwxrwx 1 root root 7 Jul 31 09:55 /dev/ups-serial -> ttyACM0

## 7. Building Docker Image

**Go to the directory with Dockerfile:** 
```bash
cd /volume1/docker/ups-to-serial/
```
**Build and run the image:**
```bash
sudo docker-compose up --build -d
```

<img width="600" alt="image" src="https://github.com/user-attachments/assets/652cdbd4-05d7-4050-a57d-111bf29202e9" />


## 8. Test

**Check via logs that the container is running and sending data**
```bash
sudo docker logs -f ups-to-serial
```
> Sent: {"battery.charge": "100", "battery.runtime": "8830", "battery.voltage": "14.1", "device.model": "UT850EG", "input.voltage": "233.0", "output.voltage": "234.0", "battery.mfr.date": "CPS", "battery.type": "PbAcid"}


**VIDEO**

<a href="https://youtu.be/IzF-i7dTb5g">
  <img src="https://img.youtube.com/vi/IzF-i7dTb5g/maxresdefault.jpg" width="600" alt="Watch the video">
</a>
