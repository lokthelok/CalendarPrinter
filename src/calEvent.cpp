/*
 *  Copyright (C) 2019 lokthelok
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
#include <calEvent.h>

//Class Function Definitions
calEvent::calEvent()
{

}

calEvent::calEvent(String response)
{
  name = getValue(response,',',nameE);
  location = getValue(response,',',locationE);
  startTime = getValue(response,',',startE);
  endTime = getValue(response,',',endE);
  allDayStr = getValue(response,',',alldayE);
  guestString = getValue(response,',',guestsE);
  guests = numGuests(response);
  guestList = new String[guests];

  if(allDayStr == String("allday"))
  {
    allDay = true;
  }
  else
  {
    allDay = false;
  }
  
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
  String out = "Calendar Event: begin\n";
  out += "  name:     " + name + "\n";
  out += "  location: " + location + "\n";
  out += "  start:    " + startTime + "\n";
  out += "  end:      " + endTime + "\n";
  String ad = allDay?String("yes"):String("no");
  out += "  allday:   " + ad + "\n";
  out += "  guests:\n";
  int i;
  for(i=0; i<guests; i++)
  {
    out += "            " + guestList[i] + "\n";
  }
  out += "Calendar Event: end\n";
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