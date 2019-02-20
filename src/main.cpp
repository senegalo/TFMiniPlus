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
  if (count == 200) {
    Serial.println("Version :" + mini.getVersion());
  }
  if (count < 202) {
    count++;
  }
}