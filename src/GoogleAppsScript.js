function doGet(){
  return ContentService.createTextOutput(GetEvents());
}

//Calendar Event Info Needed
// - Name
// - Location
// - Start Time
// - End Time (or date) eg. tomorrow
// - All Day?? (no times needed)
// - Invitees

//Calendar Information
// https://theeventscalendar.com/support/forums/topic/ics-how-to-specify-all-day-event/#post-1206905

function GetEvents(){
  var _calendarName = 'CalendarPrinter'
  var Cal = CalendarApp.getCalendarsByName(_calendarName)[0];
  var Now = new Date();
  var events = Cal.getEventsForDay(Now);
  str = "";
  for (var i = 0; i < events.length; i++)
  {
    var allDay;
    if(events[i].getStartTime().toLocaleTimeString().search("12:00:00 AM") >= 0 &&
       events[i].getEndTime().toLocaleTimeString().search("12:00:00 AM") >= 0)
    {
      allDay = "allday";
    }
    else
    {
      allDay = "notday";
    }
    var guestList = events[i].getGuestList();

    str += events[i].getTitle() + ',' ;
    str += events[i].getLocation() + ',' ;
    str += events[i].getStartTime().toLocaleTimeString() + ',' ;
    str += events[i].getEndTime().toLocaleTimeString() + ',' ;
    str += allDay + ',' ;
    for (var j = 0; j < guestList.length; j++)
    {
      if(guestList[j].getName() != _calendarName)
      {
        if(guestList[j].getName() != "") {
          str += guestList[j].getName() + ":" ;
        }
        else
        {
          str += guestList[j].getEmail() + ":" ;
        }
      }
    }
    //Remove last ":" from guest lists
    str = str.substring(0, str.length - 1);
    str += '\n';
  }
  str = str.substring(0, str.length - 1);
  return str;
}