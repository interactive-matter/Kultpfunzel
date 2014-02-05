/*
  Read analog values from a potentiometer
*/

int ledPin = 10; // And a led between ground and this pin
int potiPin = A3;
 
unsigned int analogValue;
 
void setup ()
{
  pinMode (ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop () {
  analogValue = analogRead(potiPin); // This is in the range 0 - 1023
  analogValue = map(analogValue, 0, 1023, 0, 255); // mapped to 0 - 255
  
  Serial.println(analogValue);
  analogWrite (ledPin, analogValue);
}
