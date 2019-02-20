#include <Arduino.h>
#include "TFMiniPlus.h"

TFMiniPlus mini;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial3.begin(115200);
  Serial.begin(115200);
  Serial.println("initialized Serials");
  mini.begin(&Serial3);
}

void loop() {
  if (count < 300) {
    if (mini.readData()) {
      Serial.print("Distance: ");
      Serial.println(mini.getDistance());
      Serial.print("Tempreature: ");
      Serial.println(mini.getSensorTempreture());
      Serial.print("SignalStrength: ");
      Serial.println(mini.getSignalStrength());
    }
    count++;
  }
}