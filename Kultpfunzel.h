/*
The MIT License (MIT)

Copyright (c) 2013, Interactive Matter, Marcus Nowotny

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef Kultpfunzel_h
#define Kultpfunzel_h

#include <Arduino.h> 
#include <math.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

class Kultpfunzel
{

public:
  Kultpfunzel();
  void setLightLevel(unsigned char level);
  boolean isSwitch();
  float getVoltageBatteryLeft();
  float getVoltageBatteryRight();
  void sleep();
	
private:
  uint8_t autoreset;
  unsigned long  previous_millis, interval_millis;
  float getVoltageBattery(char battery_port);
  void initializeLightLevels();
  unsigned char level;

  //some brightness levels according to gamma factor 2,2 - 
  //to have a good amount of brightness but also mapped to human vision
  //it is one less than 256 since 0 is handled differently - since it is off
  unsigned int light_levels[255];
};

#endif


