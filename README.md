# CalendarPrinter
Daily Calendar Printer (ESP32 and Serial Thermal Printer)

Latest Released Source: [download](/lokthelok/CalendarPrinter/releases/latest/download/v1.0.0.zip)

## Outline
* ESP32 scans WiFi networks and connects to first available
* ESP32 sends GET to Google Apps Script
* Google Apps Script fetches today's calendar events and creates string of events
* ESP32 reads string into calender event datatype
* ESP32 prints formatted list of the day's events

## Mechanical
See [mechanical files](mechanical/MECHANICAL.md)

## License
[GNU General Public License v3.0](LICENSE)

## References
* [Project-mmClock](https://github.com/MickMake/Project-mmClock) by MickMake
  * WebFetch(), WebGCal()
  * Google Apps Script
* [Reminder-with-Google-Calendar](https://github.com/SensorsIot/Reminder-with-Google-Calender) by Sensorslot
  * DEBUG #defines
* [ESP8266MQTTMesh](https://github.com/PhracturedBlue/ESP8266MQTTMesh) by PhracturedBlue
  * wifi_conn struct