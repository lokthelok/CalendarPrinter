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
#include <calPrint.h>
#include <calEvent.h>
#include <logo.h>

//Function Definitions
void eventPrint(calEvent *event)
{
  String out = makeLine(event->name,32,true) + "\n";
  if(!event->allDay)
  {
    out += makeLine("Start: " + event->startTime,32,false) + "\n";
    out += makeLine("End:   " + event->endTime,32,false) + "\n";
  }
  else
  {
    out += makeLine("All Day",32,false) + "\n";
  }
  if(!(event->location == String("")))
  {
    out += makeLine("At: " + event->location,32,true) + "\n";
  }
  if(event->guests > 0)
  {
    out += "Guests:\n";
    int i;
    for(i=0; i<event->guests; i++)
    {
      out += makeLine("  " + event->guestList[i],32,false) + "\n";
    }
  }
  out += "\n";
  printer.print(out);
  return;
}

void headerPrint(String date)
{
  //Print Logo
  printer.justify('C');
  printer.printBitmap(logo_width, logo_height, logo_data);
  printer.justify('L');
  printer.setLineHeight(24);

  //Print Date
  topBorder(DOUBLE, 32, 0);
  wrapBorders(date, DOUBLE, 32, 7, 0);
  bottomBorder(DOUBLE, 32, 0);
  printer.print('\n');
  printer.print('\n');
}

void wrapBorders(String input, borderStyle style, byte width, byte innerPadding, byte outerPadding)
{
  int i;
  for(i = 0; i < outerPadding; i++)
  {
    printer.write(' ');
  }
  switch(style)
  {
    case LIGHT:
      printer.write('|');
      break;
    case HEAVY:
      printer.write(0xB3);
      break;
    case DOUBLE:
      printer.write(0xBA);
      break;
    default:
      printer.write(0xB3);
      break;
  }
  for(i = 0; i < innerPadding; i++)
  {
    printer.write(' ');
  }

  //Clip Input Length
  input = input.substring(0,width - (outerPadding + innerPadding + 1) * 2);
  printer.print(input.c_str());
  for(i = input.length(); i < width - (outerPadding + innerPadding + 1) * 2; i++)
  {
    printer.print(' ');
  }

  for(i = 0; i < innerPadding; i++)
  {
    printer.write(' ');
  }
  switch(style)
  {
    case LIGHT:
      printer.write('|');
      break;
    case HEAVY:
      printer.write(0xB3);
      break;
    case DOUBLE:
      printer.write(0xBA);
      break;
    default:
      printer.write(0xB3);
      break;
  }
  for(i = 0; i < outerPadding; i++)
  {
    printer.write(' ');
  }
  return;
}

void wrapBorders(String input, borderStyle style, byte width)
{
  wrapBorders(input, style, width, 0, 0);
  return;
}

void topBorder(borderStyle style, byte width, byte outerPadding)
{
  int i;
  for(i = 0; i < outerPadding; i++)
  {
    printer.write(' ');
  }
  switch(style)
  {
    case LIGHT:
      printer.write('l');
      break;
    case HEAVY:
      printer.write(0xDA);
      break;
    case DOUBLE:
      printer.write(0xC9);
      break;
    default:
      printer.write(0xDA);
      break;
  }
  for(i = 0; i < (width - 2 * (1 + outerPadding)); i++)
  {
    switch(style)
    {
      case LIGHT:
        printer.write('-');
        break;
      case HEAVY:
        printer.write(0xC4);
        break;
      case DOUBLE:
        printer.write(0xCD);
        break;
      default:
        printer.write(0xC4);
        break;
    }
  }
  switch(style)
  {
    case LIGHT:
      printer.write('r');
      break;
    case HEAVY:
      printer.write(0xBF);
      break;
    case DOUBLE:
      printer.write(0xBB);
      break;
    default:
      printer.write(0xBF);
      break;
  }
  for(i = 0; i < outerPadding; i++)
  {
    printer.write(' ');
  }
  return;
}

void bottomBorder(borderStyle style, byte width, byte outerPadding)
{
  int i;
  for(i = 0; i < outerPadding; i++)
  {
    printer.write(' ');
  }
  switch(style)
  {
    case LIGHT:
      printer.write('l');
      break;
    case HEAVY:
      printer.write(0xC0);
      break;
    case DOUBLE:
      printer.write(0xC8);
      break;
    default:
      printer.write(0xC0);
      break;
  }
  for(i = 0; i < (width - 2 * (1 + outerPadding)); i++)
  {
    switch(style)
    {
      case LIGHT:
        printer.write('-');
        break;
      case HEAVY:
        printer.write(0xC4);
        break;
      case DOUBLE:
        printer.write(0xCD);
        break;
      default:
        printer.write(0xC4);
        break;
    }
  }
  switch(style)
  {
    case LIGHT:
      printer.write('r');
      break;
    case HEAVY:
      printer.write(0xD9);
      break;
    case DOUBLE:
      printer.write(0xBC);
      break;
    default:
      printer.write(0xD9);
      break;
  }
  for(i = 0; i < outerPadding; i++)
  {
    printer.write(' ');
  }
  return;
}

String makeLine(String input, byte width, bool wrap)
{
  if(!wrap)
  {
    input = input.substring(0,width);
    return input;
  }
  int startOfLine = 0;
  int i = 0;
  //Cleanse of \n for processing.
  for(i = 0; i < input.length(); i++)
  {
    if(input[i] == '\n')
    {
      input[i] = ' ';
    }
  }
  i = 0;
  while(i < input.length())
  {
    if(input.length() - startOfLine < width) break;
    int lastBreak = 0;
    for(i = startOfLine; i < width; i++)
    {
      if(input[i] == ' ')
      {
        lastBreak = i;
      }
    }
    //Serial.println(lastBreak);
    if(lastBreak > (startOfLine + width * 2/3))
    {
      input[lastBreak] = '\n';
      startOfLine = lastBreak + 1;
    }
    else
    {
      String left = input.substring(0,startOfLine+width);
      String right = input.substring(startOfLine+width,input.length());
      input = left + "\n" + right;
      startOfLine = startOfLine+width+1;
    }
  }

  return input;
}