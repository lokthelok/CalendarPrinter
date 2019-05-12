#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include "urls.h"

#define NO_WIFI
 
const char* ssid = SSID;
const char* password = PSK;



void listAllFiles();
String getValue(String data, char separator, int index);

void setup() {
 
  Serial.begin(115200);
  delay(1000);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  listAllFiles();
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
 
#ifndef NO_WIFI
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
    File f = SPIFFS.open("/cal.ics", "w");

    if(f)
    {
      http.begin(CAL_URL); //Specify the URL
      int httpCode = http.GET(); //Make the request
  
      if (httpCode > 0)
      { //Check for the returning code
        Serial.println(httpCode);
        if (httpCode == HTTP_CODE_OK) {
          http.writeToStream(&f);
        }
        else
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
      }
  
      else {
        Serial.println("[HTTP] GET... failed, error: 0\n");
      }
    } 
    http.end(); //Free the resources
  }
#endif

File f;

#ifdef NO_WIFI
f = SPIFFS.open("/cal.ics", "w");
f.print(NO_WIFI_CAL);
f.close();
Serial.print("Fake ical Written\n");
#endif

  //Process File
  //Search for "BEGIN:VEVENT\n", discarding lines (inclusive)
  //Save following lines until "END:VEVENT\n" (which is also discarded) in String array
  //Run a 'process event' command to:
  //  take each line saved into perhaps a string array and:
  //    Strip identifier, decode data, save to struct
  //  increment event struct array index
  //Repeat until end of file

  //Read in line to temp
  f = SPIFFS.open("/cal.ics", "r");
  String line;
  //Check if it is 'BEGIN:VEVENT'
  while(f.available())
  {
    while(line != "BEGIN:VEVENT")
    {
      line = f.readStringUntil('\n');
    }
    //Event Found
    String label[20];
    String data[20];
    uint16_t line_i = 0;
    line = f.readStringUntil('\n');
    while(line != "END:VEVENT")
    {
      for(int i=0; i < line.length(); i++)
      {
        if(line[i] == ':' || line[i] == ';')
        {
          //Found the separator
          label[line_i] = line.substring(0,i);
          data[line_i] = line.substring(i+1,line.length());
          line_i++;
          break;
        }
      }
      line = f.readStringUntil('\n');
    }

    Serial.println("\nCalendar Event:");
    for(int j=0;j<line_i;j++)
    {
      Serial.printf("%2u ",j);
      Serial.print(label[j]);
      Serial.print("-->");
      Serial.println(data[j]);
    }
  }
  
  
  
  //Print File  
  // listAllFiles();
  // File f = SPIFFS.open("/cal.ics", "r");
  // if(f)
  // {
  //   Serial.printf("File Content (%u bytes):\n", f.available());
  //   while(f.available()){
  //       Serial.write(f.read());
  //   }
  //   f.close();
  // }

  Serial.println("\nwhile(1);");
  while(1);
  delay(10000);
 
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