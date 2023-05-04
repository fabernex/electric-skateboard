#include <Arduino.h>
#include <SPI.h>
const int CS = 10;
const int throttle = A0;
int level = 135;
int neutral = 100;
int startTime = millis();



void potWrite(int value) 
{
  // Note that the integer vale passed to this subroutine
  // is cast to a byte
  value = (byte) value;
  digitalWrite(CS,LOW);
  SPI.transfer(B00010001); // This tells the chip to set the pot
  SPI.transfer(value);     // This tells it the pot position
  digitalWrite(CS,HIGH); 
  Serial.print("potWrite: ");
  Serial.println(value);
}

void brakeGradualDecrease() {
  for (int bLevel = level - 50; bLevel > 45; bLevel-=2) {
    potWrite(bLevel);
    delay(100);
  }
}

void brakeAlternateVals(int UPPER, int LOWER) {
  potWrite(UPPER);
  delay(1000);
  potWrite(LOWER);
  delay(100);
}

void setup() {
  pinMode(CS, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
}

void loop() {
  int throttle = analogRead(A0);
  double scaledValue = (double)throttle/1020 * 255;
  int roundValue = floor(scaledValue);
  Serial.print("Throttle: "); 
  Serial.println(throttle);
  Serial.print("Scaled 255 Value: ");
  Serial.println(roundValue);
  /*for (int i = 0; i < 3; i ++) {
    brakeAlternateVals(110, 49); //send alternating values to the motor to dampen brake
  }*/

  //hard stop if throttle is pulled all the way back
  if (throttle <= 9) {
    potWrite(49);
  }
  
  if (throttle <= 400 && throttle > 9) {
    brakeAlternateVals(110, 49);
  } else if (throttle <= 600 && throttle >= 480) {
    potWrite(neutral);
  } else {
    potWrite(roundValue);
  }

  
}
