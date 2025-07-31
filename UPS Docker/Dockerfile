# Použijeme základní image Debianu (odlehčenou verzi Bullseye)
FROM debian:bullseye-slim

# Aktualizace seznamu balíčků, instalace potřebného software:
# - nut-client: nástroj upsc pro čtení dat z UPS
# - python3: interpret jazyka Python 3
# - python3-pip: správce balíčků pro Python
# - pip3 install pyserial: instalace knihovny pro komunikaci po sériovém portu
# Poté smažeme nepotřebné cache a seznamy pro menší velikost image
RUN apt-get update && \
    apt-get install -y nut-client python3 python3-pip && \
    pip3 install pyserial && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Zkopírujeme náš Python skript do kořenového adresáře image
COPY ups_to_ttgo.py /ups_to_ttgo.py

# Při startu kontejneru spustíme skript pomocí Pythonu 3
CMD ["python3", "/ups_to_ttgo.py"]


