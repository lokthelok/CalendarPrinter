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

#ifndef calEvent_h
#define calEvent_h

#include <Arduino.h>

// Class Declaration
class calEvent {
  public:
    //Functions
    calEvent();
    calEvent(String response);
    ~calEvent();
    const char* stringify();

    //Variables
    String name;
    String location;
    String startTime;
    String endTime;
    bool allDay;
    String allDayStr;
    String *guestList;
    int guests;

  private:
    //Functions
    int numGuests(String response);

    //Variables
    String guestString;
};

// Enum Declarations
enum eventE
{
  nameE     = 0,
  locationE = 1,
  startE    = 2,
  endE      = 3,
  alldayE   = 4,
  guestsE   = 5
};

// Function Declarations
String getValue(String data, char separator, int index);
int getLength(String data, char separator);

#endif