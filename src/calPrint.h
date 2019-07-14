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

#ifndef calPrint_h
#define calPrint_h

#include <Arduino.h>
#include <calEvent.h>
#include <Adafruit_Thermal.h>

//Object Definition
extern Adafruit_Thermal printer;

//Typedef Enums
typedef enum borderStyle
{
  LIGHT,
  HEAVY,
  DOUBLE
}borderStyle;

//Function Declaration
void eventPrint(calEvent *event);
void headerPrint(String date);
String makeLine(String input, byte width, bool wrap);
void wrapBorders(String input, borderStyle style, byte width, byte innerPadding, byte outerPadding);
void wrapBorders(String input, borderStyle style, byte width);
void topBorder(borderStyle style, byte width, byte outerPadding);
void bottomBorder(borderStyle style, byte width, byte outerPadding);

#endif