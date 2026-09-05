#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <esp_efuse.h>
#include <esp_gap_ble_api.h>

namespace {

constexpr char kSpotServiceUUID[] =
    "A6B51001-8A5D-4F7A-9E4C-123456789101";
constexpr char kSpotIdentityCharacteristicUUID[] =
    "A6B51002-8A5D-4F7A-9E4C-123456789102";

char spotID[24] = {};

class SpotServerCallbacks final : public BLEServerCallbacks {
 public:
  void onDisconnect(BLEServer* server) override {
    BLEDevice::startAdvertising();
  }
};

SpotServerCallbacks serverCallbacks;

void configureSpotIdentity() {
  uint8_t mac[6] = {};
  esp_efuse_mac_get_default(mac);
  snprintf(
      spotID,
      sizeof(spotID),
      "TS-%02X%02X%02X%02X%02X%02X",
      mac[0],
      mac[1],
      mac[2],
      mac[3],
      mac[4],
      mac[5]);
}

void startSpotAdvertising() {
  BLEDevice::init("Telescan Spot");
  BLEDevice::setPower(ESP_PWR_LVL_P9);

  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(&serverCallbacks);

  BLEService* service = server->createService(kSpotServiceUUID);
  BLECharacteristic* identity = service->createCharacteristic(
      kSpotIdentityCharacteristicUUID,
      BLECharacteristic::PROPERTY_READ);
  identity->setValue(spotID);
  service->start();

  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(kSpotServiceUUID);
  advertising->setScanResponse(false);
  advertising->setMinInterval(160);  // 100 ms in 0.625 ms units.
  advertising->setMaxInterval(240);  // 150 ms in 0.625 ms units.
  advertising->start();
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);

  configureSpotIdentity();
  startSpotAdvertising();

  Serial.println();
  Serial.println("Telescan Spot Wake v0");
  Serial.printf("Spot ID: %s\n", spotID);
  Serial.printf("BLE service: %s\n", kSpotServiceUUID);
  Serial.println("Advertising started");
}

void loop() {
  delay(1000);
}
