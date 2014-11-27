/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  pinMode(11, OUTPUT); 
}

void loop() {


for (int i=1; i<15; i+=1)
{  
  for(int j=1; j<x;j++)
   {   
     digitalWrite(13, HIGH);   // set the LED on
     delay(10);              // wait for a second
     digitalWrite(13, LOW);    // set the LED off
     delay(i);  // wait for a second

   }
   int qq=i*10;
   analogWrite(11, qq);   
}
for (int i=15; i>1; i-=1)
{ 
  for(int j=1; j<x;j++)
   { 
    digitalWrite(13, HIGH);   // set the LED on
    delay(10);              // wait for a second
    digitalWrite(13, LOW);    // set the LED off
    delay(i);  // wait for a second
   }
   int qq=i*10;
   analogWrite(11, qq);   
}
}

