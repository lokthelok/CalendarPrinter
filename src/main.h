/*
 *  Copyright (C) 2019 lokthelok
 *  Copyright (C) 2016 PhracturedBlue
 *  Copyright (C) 2018 Andreas Spiess
 *  Copyright (C) 2018 MickMake
 *
 *  This file is part of CalendarPrinter.
 * 
 *  CalendarPrinter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <Adafruit_Thermal.h>

//Uncomment DEBUGG to print serial debug messages
// LarryD, Arduino forum
//#define DEBUGG
#ifdef DEBUGG
#define DPRINT(...)    Serial.print(__VA_ARGS__)
#define DPRINTLN(...)  Serial.println(__VA_ARGS__)
#define DPRINTF(...)   Serial.printf(__VA_ARGS__)
#else
#define DPRINT(...)
#define DPRINTLN(...)
#define DPRINTF(...)
#endif

#define WIFI_CONN(ssid, password) \
    { ssid, password }

typedef struct {
    const char *ssid;
    const char *password;
} wifi_conn;

String FetchGCal(String url);
String WebFetch(String url);
wifi_conn searchWiFi(void);

WiFiClientSecure client;
Adafruit_Thermal printer(&Serial2);
const wifi_conn networks[] = NETWORK_LIST;
wifi_conn network;

calEvent* arrangedEvents;
String* name;
String* location;
String* times;