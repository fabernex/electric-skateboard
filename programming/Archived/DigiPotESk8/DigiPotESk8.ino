#include <Arduino.h>
#include <SPI.h>
const int CS = 10;
const int throttle = A0;
int level = 135;
int noMotorLevel = 120;
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
  //Serial.println(value);
}

void brakeGradualDecrease() {
  for (int bLevel = level - 50; bLevel > 45; bLevel-=2) {
    potWrite(bLevel);
    delay(100);
  }
}

void brakeAlternateVals(int UPPER, int LOWER) {
  potWrite(UPPER);
  delay(500);
  potWrite(LOWER);
  delay(500);
}

void setup() {
  pinMode(CS, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
}

void loop() {
  int throttle = analogRead(A0);
  Serial.println(throttle);
  /*for (int i = 0; i < 3; i ++) {
    brakeAlternateVals(110, 49); //send alternating values to the motor to dampen brake
  }*/
  if (throttle <= 450) {
    brakeAlternateVals(110, 49);
  } else {
    potWrite(throttle);
  }
  
}
