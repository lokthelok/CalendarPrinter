#include <Arduino.h>
#include <calEvent.h>

//Class Function Definitions
calEvent::calEvent(String response)
{
  name = getValue(response,',',nameE);
  location = getValue(response,',',locationE);
  startTime = getValue(response,',',startE);
  endTime = getValue(response,',',endE);
  allDay = getValue(response,',',alldayE);
  guestString = getValue(response,',',guestsE);
  guests = numGuests(response);
  guestList = new String[guests];

  int i;
  for(i=0; i<guests; i++)
  {
    guestList[i] = getValue(guestString,':',i);
  }
};

calEvent::~calEvent()
{
  delete [] guestList;
}

int calEvent::numGuests(String response)
{
  String guestString = getValue(response,',',guestsE);
  return getLength(guestString,':');
}

const char* calEvent::stringify() {
  String out = "calEvent: begin\n";
  out += "  name:     " + name + "\n";
  out += "  location: " + location + "\n";
  out += "  start:    " + startTime + "\n";
  out += "  end:      " + endTime + "\n";
  String ad = allDay?String("allday"):String("notday");
  out += "  allday:   " + ad + "\n";
  out += "  guests:\n";
  int i;
  for(i=0; i<guests; i++)
  {
    out += "            " + guestList[i] + "\n";
  }
  out += "calEvent: end\n";
  return out.c_str();
}

//Function Definitions
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
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

int getLength(String data, char separator)
{
  int found = 0;
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
    }
  }
  return found;
}