# Acurite 5n1 -> Weather Services Forwarder

Receives Acurite 5n1 station measurements and uploads them to multiple weather services.

* [How to assemble](#assemble).
* [How to configure data export](#configuring-data-export).

<img src="docs/front.png" width="500" alt="front">

Supported weather services:

* [PWS Weather](https://www.pwsweather.com/)
* [Weather Underground](https://www.wunderground.com/)
* [Windy.com](https://stations.windy.com/)
* [Windguru](https://www.windguru.cz/)

Based on [TTGO / LILYGO LoRa32](https://www.lilygo.cc/products/lora3) (ESP32 + SX1278 433 Mhz transceiver).

## Assemble

1. Connect a button between GPIO4 and GND. <img src="docs/disassembled.png" width="500" alt="disassembled">
2. Connect ESP32 to PC.
3. Run  `Upload Filesystem Image` and `Upload` PlatformIO targets
4. Find ESP32 started and showing info.

## Configuring data export

1. Unplug power cable from device.
2. Press and hold a button (connect GPIO4 to GND).
3. Plug the power cable back.
4. Once device starts, find Wi-Fi AP Settings and IP address.
5. Connect to displayed AP, open IP address in web browser (by default there are `WBridge-AP` non-secured network and
   most likely it'll be [192.168.4.1](http://192.168.4.1/)).
6. Enter your Wi-Fi Network Name (SSID) and Password.
7. Select *Weather Station* from the list (the device will listen for station signals, to get updated list refresh the
   page). Currently supported Acurite 5n1 only.
8. Fill weather service credentials. Missing credentials will simply
9. If you're not in UTC timezone, please specify your timezone in POSIX format, see
   the [documentation](https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html).
10. Click on the "SAVE SETTINGS" button.
11. Find your device rebooted and using the new settings.

## Many thanks to

* [Lopaka.app](https://lopaka.app/) which helped designing and developing display screens extremely easy and fast.
* [JesAde](https://www.thingiverse.com/jesade/designs)
  for [LoRa32 module case 3D model](https://www.thingiverse.com/thing:4518757).
* [NorthernMan54](https://github.com/NorthernMan54) for
  his [port of rtl_433 library](https://github.com/NorthernMan54/rtl_433_ESP) for ESP.

## Notes

* The code is written not really efficient, consumes many memory. But this wasn't a plan to have it efficient, I wanted
  to have it simply working in short time :)
* To reduce memory consumption, the code contains
  shrunk [port of rtl_433 library](https://github.com/NorthernMan54/rtl_433_ESP) copied directly to [lib/](lib).
* It's pretty easy to support other stations (not Acurite 5n1 only), but there was not really reasonable to have this in
  current use case.
* Precipitation measurements from Acurite 5n1 are received in weird time window, to be investigated whether they're
  interpreted correctly. The device may report garbage instead of `rainmm` measurements to weather services. 

