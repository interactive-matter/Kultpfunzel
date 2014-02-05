/* Crazy shit 8-bit symphony generator */
 
/*
 * inspired by:
 *  http://countercomplex.blogspot.com/2011/10/algorithmic-symphonies-from-one-line-of.html
 */

#define switchPin 8

int speakerPin = 3; // Connect a piezo sounder between ground and this pin
int ledPin = 10; // And a led between ground and this pin
int potiPin = A3;
 
long t = 0;
int v = 0;
int c = 0;
 
unsigned int analogValue;
 
void setup ()
{
  pinMode(switchPin, INPUT); // Switch 1
  pinMode (speakerPin, OUTPUT);
  pinMode (ledPin, OUTPUT);
  
  Serial.begin(9600);
}


void loop () {

  // variant A
  for (t=0;;t++) {
    c = (analogRead(potiPin)>>3); 
    v = 1000*t*((t>>14|t>>(8))&c&t>>c);
    // if (isSwitch()) { // only do this if the switch is pressed
      analogWrite (ledPin, v);
      analogWrite (speakerPin, v);
      // Serial.println(analogRead(potiPin));
    // }
  }  
    
  /*
  // variant B
  for (t=0;;t++){
    c = (analogRead(potiPin)>>4); 
    v = t*((t>>5|t>>11)&c&t>>c);
    analogWrite (ledPin, v);
    // analogWrite (speakerPin, v);
  }
  */

  /*
  //variant C
  for (t=0;;t++){
    c = (analogRead(potiPin)>>4); 
    v = t*((t>>7|t>>(c+4))&(c)&t>>(c*3));
    analogWrite (speakerPin, v);
  }
  */
  
}

// Check if the switch is pressed (From original Pfunzel sketch)
boolean isSwitch() {
  return PINE & _BV(2);
}
