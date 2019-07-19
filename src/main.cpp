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

#include <WiFi.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <Adafruit_Thermal.h>
#include <calEvent.h>
#include <calPrint.h>
#include <algorithm>
#if __has_include(<urls.h>)
#  include <urls.h>
#  define URLS
#endif

#ifndef URLS
//Add or remove full WIFI_CONN() lines to add/remove networks from list
//Networks are connected by ordered preference, index 0 being first.
#define NETWORK_LIST { \
	WIFI_CONN("SSID1", "PASSWORD1"), \
	WIFI_CONN("SSID2", "PASSWORD2"), \
	WIFI_CONN("SSID3", "PASSWORD3"), \
	NULL, \
	}
#define NETWORK_LIST_LENGTH 3
const char* script = "https://script.google.com/macros/s/A1B2C3D4/exec";
#else
const char* script = SCRIPT_URL;
#endif

#include <main.h>

void setup() {
	//Initialise Serial
	Serial.begin(115200);

	//Initialise Thermal Printer
	Serial2.begin(9600);
	printer.begin();

	//Connect to first network available in list
	network = searchWiFi();
	DPRINT("Connecting to WiFi.");
	WiFi.begin(network.ssid, network.password);
	//Wait for WiFi connection
	while (WiFi.status() != WL_CONNECTED) {
		DPRINT(".");
		delay(300);
	}
	DPRINTF("\nConnected to WiFi: %s\n",network.ssid);
 
}
 
void loop() {
	//Wait for WiFi connection (shouldn't be necessary)
  	if (WiFi.status() == WL_CONNECTED)
	{
		//Get Google Scripts calendar response
		String response = FetchGCal(script);
		DPRINTF("Today's Calendar:\n%s\n", response.c_str());
		
		//Extract current date
		String date = getValue(response,'\n',0);
		date = date.substring(0,15);
		DPRINTF("Extracted Date: %s\n",date.c_str());

		//Get number of calendar events
		byte eventsLength = getLength(response,'\n') - 1;
		DPRINTF("Number Events: %u\n",eventsLength);

		//Create calEvent array and randomise
		int i;
		arrangedEvents = new calEvent[eventsLength];
		name = new String[eventsLength];
		location = new String[eventsLength];
		times = new String[eventsLength*2];

		if(eventsLength > 0)
		{	
			//Load Arrays
			for(i = 0; i < eventsLength; i++)
			{
				arrangedEvents[i] = calEvent(getValue(response,'\n',i+1));
				name[i] = arrangedEvents[i].name;
				location[i] = arrangedEvents[i].location;
				times[i] = arrangedEvents[i].startTime;
				times[i+eventsLength] = arrangedEvents[i].endTime;
			}
			
			//Randomize Data
			std::random_shuffle(&name[0],&name[eventsLength],static_cast<long(*)(long)>(random));
			std::random_shuffle(&location[0],&location[eventsLength],static_cast<long(*)(long)>(random));
			std::random_shuffle(&times[0],&times[eventsLength*2],static_cast<long(*)(long)>(random));

			//Random Data back to Events
			for(i = 0; i < eventsLength; i++)
			{
				arrangedEvents[i].name = name[i];
				arrangedEvents[i].location = location[i];
				arrangedEvents[i].startTime = times[i];
				arrangedEvents[i].endTime = times[i+eventsLength];
			}
		}

		//Print Today's Events
		printer.wake();
		printer.print('\n');
		headerPrint(date);
		for(i = 0; i < eventsLength; i++)
		{
			DPRINTLN(arrangedEvents[i].stringify());
			eventPrint(&arrangedEvents[i]);
		}
		printer.feed(3);

		//Printer to sleep
		printer.sleep();
		DPRINTLN("\nwhile(1);");
  		while(1);
	}
}

String FetchGCal(String url)
{
	String Return1;
	String Return2;
	//Get redirect notice
	Return1 = WebFetch(url);
	//Get calendar
	Return2 = WebFetch(Return1);

	return(Return2);
}

String WebFetch(String url)
{
	const char *strURL;
	String Response;
	char server[80];
	bool Redirect = 0;

	strURL = url.c_str();
	DPRINT("GCAL:URL:");
    DPRINTLN(strURL);

	if (memcmp("https://", strURL, 8) == 0)
	{
		int i;
		for(i=0; i<80; i++)
		{
			if (strURL[i+8] == '/')
				break;
			server[i] = strURL[i+8];
		}
		server[i] = 0;
	}

	DPRINT("GCAL:server:");
    DPRINTLN(server);
	if (!client.connect(server, 443))
		DPRINTLN("GCAL:No connection");
	else
	{
		DPRINTLN("GCAL:Connect");
		// Make a HTTP request:
		client.print("GET ");
		client.print(url);
		client.println(" HTTP/1.0");

		client.print("Host: ");
		client.println(server);
		client.println("Connection: close");
		client.println();

		String header;
		while (client.connected())
		{
			String line = client.readStringUntil('\n');
			header = header + line + "\n";
			if (line.startsWith("Location: "))
			{
				Redirect = 1;
				Response = line.substring(line.indexOf("http"));
				DPRINT("GCAL:REDIRECT:");
        		DPRINTLN(Response);
			}
			if (line == "\r")
				break;
		}
		DPRINT("GCAL:HEADER:");
    	DPRINTLN(header);

		String body;
		while (client.available())
		{
			String line = client.readStringUntil('\n');
			body = body + line + "\n";
			if (line == "\r")
				break;
		}

		if (!Redirect)
		{
			DPRINT("GCAL:BODY:");
      		DPRINTLN(body);
			Response = body;
		}
		client.stop();
	}
	return(Response);
}

wifi_conn searchWiFi(void)
{
	DPRINTF("scan start\n");
	int n = WiFi.scanNetworks();
	DPRINTF("scan done: %i\n", n);
	for(int x = 0; x < n; x++) DPRINTF("%i:%s\n",x,WiFi.SSID(x).c_str());

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	wifi_conn available_network;
	int i,j;
	if(n > 0)
	{
		for(i = 0; i < NETWORK_LIST_LENGTH; i++)
		{
			DPRINTF("l");
			for(j = 0; j < n; j++)
			{
				DPRINTF("f");
				if(strcmp(networks[i].ssid,WiFi.SSID(j).c_str()) == 0)
				{
					DPRINTF("\nMatched - list: %s  found: %s\n",networks[i].ssid,WiFi.SSID(j).c_str());
					available_network = networks[i];
					return available_network;
				}
			}
		}
	}
	return available_network;
}