/*
  Button
 
  Turns the onboard LED on when pressing the switch "SW"

*/

#define switchPin 8 // The switch "SW" is on digital pin 8
#define ledPin 10 // The LED is on digital pin 10

void setup() {
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  if (isSwitch()) {
    digitalWrite(ledPin, 255);
    Serial.println("The Kultpfunzel says: Button gedrueckt");
  } else {
    digitalWrite(ledPin, 0);
  }
}

// Helper function from original Pfunzel sketch
boolean isSwitch() {
  return PINE & _BV(2);
}

