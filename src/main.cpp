#include <Arduino.h>
#include "TFMiniPlus.h"

TFMiniPlus mini;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.begin(9600);
  //Serial.println("initialized Serials");
  mini.begin(&Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  char a;
  while(Serial.available() > 0){
    a = Serial.read();
  }
  /*
  if(mini.readData()){
    Serial.println("Distance: " + mini.getDistance());
  }*/
}