/*
 * ReadData.ino
 *
 *  Created on: Feb 24, 2019
 *      Author: Karim Mansour
 * Example on how to read data from the TFMiniPlus.
 * More info available in the github readme
 * Released under the GPL-3.0
 *
 * https://github.com/senegalo/tfmini-plus
 */

#include "TFMiniPlus.h"

TFMiniPlus tfmini;

void setup() {
  // Start serial port to communicate with the TFMini
  // Default baud rate is 115200
  Serial.begin(115200);

  // Pass the Serial class initialized to the tfmini
  tfmini.begin(&Serial);
}

void loop() {
  // read the data frame sent by the mini
  if (tfmini.readData()) {
    // Distance "default in CM"
    tfmini.getDistance();

    // Sensor tempreture in celsius
    tfmini.getSensorTempreture();

    // Signal Strength
    tfmini.getSignalStrength();
  }
}