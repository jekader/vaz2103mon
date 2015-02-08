/*
  vaz2103mon - displays CPU stats on a VAZ 2103 instrument panel
  
  This code is in the public domain.
 */

// driving cd-rom stepper, based on
// http://www.edaboard.com/thread217270.html

// define stepper pins (they don't need to be PWM-capable)
// I'm using a 28YBT-48 stepper
// some info is available here: http://www.utopiamechanicus.com/article/arduino-stepper-motor-setup-troubleshooting/
int coila1 = 2; 
int coila2 = 4; 
int coilb1 = 3; 
int coilb2 = 5;
// delay between steps
int mdelay = 10;

// define PWM pins for remaining gauges
int rampin = 9;
int netpin = 10;
int temppin = 11;
int iopin = 6;

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  // init PWM pins
  pinMode(6, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  // init stepper pins
  pinMode(coila1, OUTPUT); 
  pinMode(coila2, OUTPUT); 
  pinMode(coilb1, OUTPUT); 
  pinMode(coilb2, OUTPUT); 

  //init serial
  Serial.begin(9600);
}

void loop() {

  // some info on PWM here: http://www.uchobby.com/index.php/2008/02/12/arduino-analog-gauge/
// need to add resistors
 if (Serial.available() > 0) {
// parsing values http://arduino.cc/en/Tutorial/ReadASCIIString
// we expect a string like 123,123,123\n on the console:
// echo "123,123,123" > /dev/ttyUSB0
    int cpu = Serial.parseInt();
    int ram = Serial.parseInt();    
    int net = Serial.parseInt(); 
    int temp = Serial.parseInt(); 
    int io = Serial.parseInt(); 

    if (Serial.read() == '\n') {
      
      if (cpu >= -2000) {
        if (cpu < 2000) {
          int sdirection = -1;
          int modcpu = cpu;
          if (cpu < 0) {
              sdirection = 1;
              modcpu = -1 * cpu;
          }
          // move only if there's more than 0 steps
          if (modcpu > 0) {
            stepperMove(sdirection,modcpu);
          }
          // otherwise disable all stepper pins
          else {
            digitalWrite(coila1, LOW); 
            digitalWrite(coila2, LOW); 
            digitalWrite(coilb1, LOW); 
            digitalWrite(coilb2, LOW);
          } 
        }
      }
      if (ram >= 0) {
        if (ram < 256) {
          analogWrite(rampin, ram); 
        }
      }
      if (net >= 0) {
        if (net < 256) {
          analogWrite(netpin, net); 
        }
      }
      if (temp >= 0) {
        if (temp < 256) {
          analogWrite(temppin, net); 
        }
      }
      if (io >= 0) {
        if (io < 256) {
          analogWrite(iopin, net); 
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
      Serial.print(printed_line);
      gauge_name = " TEMP:";
      gauge_value = String(temp);
      printed_line = gauge_name + gauge_value;
      Serial.print(printed_line);
      gauge_name = " IO:";
      gauge_value = String(io);
      printed_line = gauge_name + gauge_value;
      Serial.println(printed_line);
    }
 }
}


// simple stepper function
int stepperMove(int rotation, int steps){

  for (int i=0; i <= steps; i++){
    if ( rotation==1){
      digitalWrite(coila1, HIGH); 
      digitalWrite(coila2, LOW); 
      digitalWrite(coilb1, HIGH); 
      digitalWrite(coilb2, LOW); 
      delay(mdelay); 

      digitalWrite(coila1, HIGH); 
      digitalWrite(coila2, LOW); 
      digitalWrite(coilb1, LOW); 
      digitalWrite(coilb2, HIGH); 
      delay(mdelay); 

      digitalWrite(coila1, LOW); 
      digitalWrite(coila2, HIGH); 
      digitalWrite(coilb1, LOW); 
      digitalWrite(coilb2, HIGH); 
      delay(mdelay); 

      digitalWrite(coila1, LOW); 
      digitalWrite(coila2, HIGH); 
      digitalWrite(coilb1, HIGH); 
      digitalWrite(coilb2, LOW); 
      delay(mdelay);
     }
    else
     {
      digitalWrite(coila1, LOW); 
      digitalWrite(coila2, HIGH); 
      digitalWrite(coilb1, HIGH); 
      digitalWrite(coilb2, LOW); 
      delay(mdelay);

      digitalWrite(coila1, LOW); 
      digitalWrite(coila2, HIGH); 
      digitalWrite(coilb1, LOW); 
      digitalWrite(coilb2, HIGH); 
      delay(mdelay);   

      digitalWrite(coila1, HIGH); 
      digitalWrite(coila2, LOW); 
      digitalWrite(coilb1, LOW); 
      digitalWrite(coilb2, HIGH); 
      delay(mdelay); 

      digitalWrite(coila1, HIGH); 
      digitalWrite(coila2, LOW); 
      digitalWrite(coilb1, HIGH); 
      digitalWrite(coilb2, LOW); 
      delay(mdelay);
     }
  }
  return 0;
}

