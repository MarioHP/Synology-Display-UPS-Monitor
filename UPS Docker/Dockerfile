# Použijeme odlehčený Debian Bullseye jako základ
FROM debian:bullseye-slim

# Nastavení prostředí – potlačí interaktivní dotazy během instalace
ENV DEBIAN_FRONTEND=noninteractive

# Instalace závislostí a vyčištění mezipaměti pro menší image
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        nut-client \
        python3 \
        python3-pip \
    && pip3 install --no-cache-dir pyserial \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Zkopíruj Python skript do image
COPY ups_to_ttgo.py /ups_to_ttgo.py

# Spusť skript po startu kontejneru
CMD ["python3", "/ups_to_ttgo.py"]

