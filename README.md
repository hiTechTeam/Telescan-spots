# Telescan Spots

Prototype firmware for the physical BLE trigger used to improve Telescan
discovery when iPhones are in the background.

The first version only advertises a dedicated Spot service. It does not change
the existing phone-to-phone BLE protocol and does not use the API.

## ESP32 prototype

```sh
cd firmware/esp32
uvx --from platformio platformio run
uvx --from platformio platformio run --target upload \
  --upload-port /dev/cu.usbserial-10
uv run --with pyserial python ../../scripts/check_serial.py \
  /dev/cu.usbserial-10
```

The serial log prints the permanent development Spot ID derived from the
ESP32 eFuse MAC.
