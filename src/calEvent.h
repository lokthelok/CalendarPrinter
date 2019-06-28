#ifndef calEvent_h
#define calEvent_h

#include "Arduino.h"

// Class Declaration
class calEvent {
  public:
    //Functions
    calEvent(String response);
    ~calEvent();
    const char* stringify();

    //Variables
    String name;
    String location;
    String startTime;
    String endTime;
    bool allDay;
    String *guestList;

  private:
    //Functions
    int numGuests(String response);

    //Variables
    int guests;
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