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
#include <Metro.h>
#include <Kultpfunzel.h>
#include <avr/power.h>

//this metro will check the swith every 50ms
Metro pinMetro = Metro(50,true);
//and this metro will give the battery voltage every minute 
Metro batteryMetro = Metro(60000ul);
//and every 5 ms we will check if the light level has to change
Metro lightMetro = Metro(5);

//the current level of the lamp light
unsigned char current_light_level=0;
//the target level of the lamp ligth - the lamp will fade from current totarget
unsigned char target_light_level=0;
//a quick switch to prevent the lamp from falling asleep
boolean stay_on = false;

//the pfunzel
Kultpfunzel pfunzel;

void setup() {
  //start the serial port
  Serial.begin(9600);
  //and display some nice welcome message
  Serial.println("This is the Kultpfunzel");
  //we do not need the PWI
  power_twi_disable();
}

void loop() {
  //check if we have to check the input switch
  if (pinMetro.check()) {
    //if the switch is set we go to full brightness
    if (pfunzel.isSwitch()) {
      setLampBrightness(255);
    } 
    else {
      //else we switch off thelight
      setLampBrightness(0);
    }
  }
  //check if we have to change the light level
  checkLamp();
  //if it is time to check the battery we will have to do so
  if (batteryMetro.check()) {
    //read an output the battery status
    Serial.println("Battery Status:");
    //the battery voltage is given as convenient floating point given directly the voltage
    float batteryLeft = pfunzel.getVoltageBatteryLeft();
    float batteryRight = pfunzel.getVoltageBatteryRight();
    Serial.print("Battery left: ");
    Serial.println(batteryLeft);
    Serial.print("Battery right: ");
    Serial.println(batteryRight);
    Serial.println();
  }
  if (!stay_on && target_light_level==0 && current_light_level==0) {
    //if we do not output any light we can go asleep
    pfunzel.sleep();
    //we need a short delay in order to ensure that 
    //the pinMetro can catch up eventually
    delay(10);
  }
}

//if we want to set the brigthness we adjust the target - so that we can slowly fade there
void setLampBrightness(unsigned char level) {
  target_light_level=level;
}

//check if we have to adjust the brightness
void checkLamp() {
  //which can only occur in certain intervals and if the target is different from the current level
  if (lightMetro.check() && (target_light_level!=current_light_level)) {
    //if the current level is below the target level
    if (target_light_level>current_light_level) {
      //increase it
      current_light_level++;
    } 
    else {
      //if not decrease it
      current_light_level--;
    }
    //finally tell the pfunzel to go to that level
    pfunzel.setLightLevel(current_light_level);
  }
}






