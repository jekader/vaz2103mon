/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
int ledPin = 13;

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  //init the PWM pins
  pinMode(3, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  //init serial
  Serial.begin(9600);
}

void loop() {

// this part is from http://www.suffix.be/blog/send-data-to-arduino
// some info on PWM here: http://www.uchobby.com/index.php/2008/02/12/arduino-analog-gauge/
// need to add resistors
 if (Serial.available() > 0) {
//    char value = Serial.read();
    int value = Serial.parseInt();
    if (value > 1) {
      if (value < 256) {
        analogWrite(11, value); 
      }
    }
/*    if (value == '1') {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    } */
    Serial.println(value);
  }

}

