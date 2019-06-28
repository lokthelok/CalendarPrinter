#include <WiFi.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <calEvent.h>
#if __has_include(<urls.h>)
#  include <urls.h>
#  define URLS
#endif

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

#ifndef URLS
const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* script = "https://script.google.com/macros/s/A1B2C3D4/exec";
#else
const char* ssid = SSID;
const char* password = PSK;
const char* script = SCRIPT_URL;
#endif

String FetchGCal(String url);
String WebFetch(String url);

WiFiClientSecure client;


void setup() {
 
  Serial.begin(115200);
  delay(1250);

  if (!SPIFFS.begin(true)) {
    DPRINTLN("Error mounting SPIFFS");
  }
  
  //listAllFiles();
  //DPRINTF("Formatted: %u\n", int(SPIFFS.format()));
  //listAllFiles();

  DPRINT("Connecting to WiFi.");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    DPRINT(".");
    delay(300);
  }
  DPRINTF("\nConnected to WiFi: %s\n",ssid);
 
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED)
	{
		String response = FetchGCal(script);
		DPRINTF("Today's Calendar:\n%s\n", response.c_str());
		
    //Extract single event for testing
    String line = getValue(response,'\n',4);
    DPRINTF("Extracted Event: %s\n",line.c_str());

    //Create CalEvent
    calEvent ev0(line);
    Serial.println(ev0.stringify());
	}

  DPRINTLN("\nwhile(1);");
  while(1);
  delay(10000);
 
}

String FetchGCal(String url)
{
	String Return1;
	String Return2;
	Return1 = WebFetch(url);
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