#include <WiFi.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#if __has_include(<urls.h>)
#  include <urls.h>
#  define URLS
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

WiFiClientSecure client;

void listAllFiles();
String getValue(String data, char separator, int index);
String FetchGCal(String url);
String WebFetch(String url);

void setup() {
 
  Serial.begin(115200);
  delay(1000);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  //listAllFiles();
  //Serial.printf("Formatted: %u\n", int(SPIFFS.format()));
  //listAllFiles();

#ifndef NO_WIFI
  delay(1000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
#endif
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED)
	{
		String response = FetchGCal(script);
		Serial.print("GCAL:");
    Serial.println(response);
		//process(response);
	}

  Serial.println("\nwhile(1);");
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
	Serial.print("GCAL:URL:");
  Serial.println(strURL);

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

	Serial.print("GCAL:server:");
  Serial.println(server);
	if (!client.connect(server, 443))
		Serial.println("GCAL:No connection");
	else
	{
		Serial.println("GCAL:Connect");
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
				Serial.print("GCAL:REDIRECT:");
        Serial.println(Response);
			}
			if (line == "\r")
				break;
		}
		Serial.print("GCAL:HEADER:");
    Serial.println(header);

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
			Serial.print("GCAL:BODY:");
      Serial.println(body);
			Response = body;
		}

		client.stop();
	}

	return(Response);
}

void listAllFiles(){
  Serial.println("----Listing SPIFFS files----");
  File root = SPIFFS.open("/");
 
  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
  Serial.println("------------end-------------"); 
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}