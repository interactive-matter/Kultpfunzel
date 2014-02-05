/*
  Blink
  Turns the white LED on for one second, then off for one second,
  repeatedly.
 */
 
// The white on-board LED is on pin 10
int led = 10;

// The setup routine runs once when you press reset:
void setup() {              
  // Initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}

// The loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
