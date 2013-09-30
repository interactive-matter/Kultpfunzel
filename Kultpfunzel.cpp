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
#include "Kultpfunzel.h"

#define LAMP_PIN 10
#define SWITCH_PIN 8

#define ANALOG_PIN_BATTERY_RIGHT 5
#define ANALOG_PIN_BATTERY_LEFT 4

#define ENABLE_OC4B_A (_BV(COM4B1) | _BV(PWM4B))
#define DISABLE_OC4B_A (_BV(PWM4B))
#define ENABLE_OC4B_B (_BV(CS40))
#define DISABLE_OC4B_B (0)

byte last_usb_frame  = 0xff;
boolean usb_communication_active = false;

 Kultpfunzel::Kultpfunzel() {
  //disable the watchdog
  MCUSR = 0;
  wdt_disable();
  //initialize the watchdog
  cli(); //it is best to not get disturbed by interrupts
  //reset the watchdog timer
  wdt_reset();
  //enable the watchdog for writing
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDCE) | _BV(WDP2) | _BV(WDIE); //enable the prescaler change, enable the interrupt, 0,25s checking interval
  //enalbe interrupts again
  sei();

  //set the switch pins as input
  //switch 1 is PE2 which is normally not usable
  DDRE |= _BV(2);
  PORTE &= ~(_BV(2));
  //the lamp is output
  pinMode(LAMP_PIN,OUTPUT);
  digitalWrite(LAMP_PIN,0);
  //we want to toggle OCR1A & 16 bit fast PWM (WGM13 |WGM12 | WGM 11)
  //fastest mode possible only CS10
  TCCR4D = 0;
  TCCR4A = DISABLE_OC4B_A;
  TCCR4B = DISABLE_OC4B_B;
  //Calculate the gamma corrected brightness values
  initializeLightLevels();
  //we want to count to the highest number
  TC4H = light_levels[254] >>8;
  OCR4C = (unsigned char)light_levels[254];
}

void Kultpfunzel::setLightLevel(unsigned char level) {
  this->level=level;
  //if it is off completely switch it off
  if (level==0) {
    //Disable the OC3A
    TCCR4A = DISABLE_OC4B_A;
    TCCR4B = DISABLE_OC4B_B;
  } 
  else{
    unsigned int light_level = light_levels[level-1];
    TCCR4E |= _BV(TLOCK4);
    //enable TCC3A
    TCCR4A = ENABLE_OC4B_A;
    TCCR4B = ENABLE_OC4B_B;
    TC4H = light_level >>8;
    OCR4B = (unsigned char)(light_level & 0xFF);
    TCCR4E &= ~(_BV(TLOCK4));
  }
}

boolean Kultpfunzel::isSwitch() {
  return PINE & _BV(2);
}

float Kultpfunzel::getVoltageBatteryRight() {
  return getVoltageBattery(ANALOG_PIN_BATTERY_RIGHT);
}

float Kultpfunzel::getVoltageBatteryLeft() {
  return getVoltageBattery(ANALOG_PIN_BATTERY_LEFT);
}

float Kultpfunzel::getVoltageBattery(char battery_port) {
  //set the analog reference to 2.56V for the batteries
  analogReference(INTERNAL);
  int battery_raw = analogRead(battery_port);
  float result = (2.56*battery_raw)/1024.0;
  //set the analog reference back to default
  analogReference(DEFAULT);
  return result;
}

void Kultpfunzel::sleep() {
  //only sleep if we are not connected to usb
  //TODO better than (UDINT & _BV(SUSPI)) &&  ?
  if (!usb_communication_active && level==0) {
    sei();
    //power_adc_disable();
    //power_twi_disable();
    sleep_enable();
    Serial.flush();
    //clear the suspi
    //UDINT &= ~(_BV(SUSPI));
    //disable the leds
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    //TODO why doen't it work??
    sleep_cpu();
    sleep_disable();
    //power_adc_enable();
    //power_twi_enable();
    sei();
  }
}

ISR(WDT_vect) {
  byte frame = UDFNUML;
  if (frame != last_usb_frame) {
    usb_communication_active=true;
  } else {
    usb_communication_active=false;
  }
  last_usb_frame=frame;
  sleep_disable();
}

void Kultpfunzel::initializeLightLevels() {
  const double gamma_factor = 2.2;
  const double correction_factor = 1023.0/pow(254.0,gamma_factor);
  //we can test for 0 since we will get an overflow anyway
  for (unsigned char brightness=1;brightness<255;brightness++) {
    double correctedLevel=pow((double)brightness,gamma_factor)*correction_factor;
    light_levels[brightness]=(unsigned int)correctedLevel;
  }
  light_levels[0]=0;
}

