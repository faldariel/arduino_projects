/*
 * Control the speed of a small DC Motor using a potentiometer.
 * 
 * The motor used in this example is a M260, running on 1-3V. 
 * The circuit uses a TIP120 and a 3V external power source.
 * 
 * The trimmer potentiometer used in our demo is 1k ohm, but larger
 * potentiometers should be possible to use.
*/
     
int motorPin = 11;
int potPin = A0; // Trimpot using Analog port.

void setup() 
{ 
   pinMode(motorPin, OUTPUT);
   // Since the potentiometer's value is used to control the motor, it is used 
   // as an input rather than a resistor.
   pinMode(potPin, INPUT);
   Serial.begin(9600);
}
     
void loop() 
{ 
  int reading = analogRead(potPin);
  int delayBeforeRotationOff = 1023-reading; // Trimpot values from 0-1023
  int delayBeforeRotationOn = reading; 

/*
 * Speed control works through 'duty cycling', where a longer
 * delay before feeding the motor will result in slower rotation
 * speed. 
 */
  digitalWrite(motorPin, HIGH); 
  delayMicroseconds(delayBeforeRotationOff);   
  digitalWrite(motorPin, LOW);  
  delayMicroseconds(delayBeforeRotationOn);
} 
