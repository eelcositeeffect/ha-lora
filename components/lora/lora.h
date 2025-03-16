#include "esphome.h"
#include <SPI.h>
#include <LoRa.h>

class LoRaSensor : public PollingComponent, public Sensor {
 public:
  LoRaSensor() : PollingComponent(5000) {}  // Update elke 5 sec

  void setup() override {
    SPI.begin(18, 19, 23, 5);  // CLK, MISO, MOSI, NSS
    LoRa.setPins(5, 14, 26);   // NSS, RST, DIO0
    if (!LoRa.begin(433E6)) {
      ESP_LOGE("LoRa", "LoRa init mislukt! Check bekabeling.");
    }
  }

  void update() override {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String received = "";
      while (LoRa.available()) {
        received += (char)LoRa.read();
      }
      ESP_LOGD("LoRa", "Pakket ontvangen: %s", received.c_str());
      publish_state(received.length());  // Geeft de lengte van het pakket weer
    }
  }
};
