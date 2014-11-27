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
// parsing values http://arduino.cc/en/Tutorial/ReadASCIIString
// we expect a string like 123,123,123\n on the console:
// echo "123,123,123" > /dev/ttyUSB0
    int cpu = Serial.parseInt();
    int ram = Serial.parseInt();    
    int net = Serial.parseInt(); 
    if (Serial.read() == '\n') {
      
      if (cpu > 0) {
        if (cpu < 256) {
          analogWrite(11, cpu); 
        }
      }
      if (ram > 0) {
        if (ram < 256) {
          analogWrite(10, ram); 
        }
      }
      if (net > 0) {
        if (net < 256) {
          analogWrite(9, net); 
        }
      }
      String gauge_name = " CPU:";
      String gauge_value = String(cpu);
      String printed_line = gauge_name + gauge_value;
      Serial.print(printed_line);
      gauge_name = " RAM:";
      gauge_value = String(ram);
      printed_line = gauge_name + gauge_value;
      Serial.print(printed_line);
      gauge_name = " NET:";
      gauge_value = String(net);
      printed_line = gauge_name + gauge_value;
      Serial.println(printed_line);


    }
 }
}

