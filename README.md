# Useless CalendarPrinter
Daily Calendar Printer (ESP32 and Serial Thermal Printer)
This device will print a receipt of your day's activities but not in a useful way. You'll have to unjumble all the information first...

Project documented on [HACKADAY.IO](https://hackaday.io/project/166659-calendar-printer-useless-edition)
Mechanical also available on [Fusion360 Gallery](https://gallery.autodesk.com/fusion360/projects/141677/calendarprinter)

## Outline
* ESP32 scans WiFi networks and connects to first available
* ESP32 sends GET to Google Apps Script
* Google Apps Script fetches today's calendar events and creates string of events
* ESP32 reads string into calender event datatype
* Randomly mixes up all information
* ESP32 prints formatted list of the day's (non)events

If there are no events in a day, the device thoughtfully gives you something to do by feeding it's entire roll out at you.

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
* [bitmapImageConvert](https://github.com/adafruit/Adafruit-Thermal-Printer-Library/tree/master/processing/bitmapImageConvert) by Adafruit
  * Receipt logo