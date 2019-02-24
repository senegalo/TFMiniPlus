/*
 * ChangeConfiguration.ino
 *
 *  Created on: Feb 24, 2019
 *      Author: Karim Mansour
 * Example on how to change the configuration of the TFMiniPlus.
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

  // Get firmware version
  tfmini.getVersion();

  // System Reset
  tfmini.systemReset();

  // Set the data frame rate
  tfmini.setFrameRate(100);

  // If frame rate is 0 use this method to trigger a data frame
  tfmini.triggerDetection();

  // Set ouput distance format to Centimiters
  tfmini.setMeasurementTo(TFMINI_MEASUREMENT_CM);

  // Set output distance format to Milimeters
  tfmini.setMeasurementTo(TFMINI_MEASUREMENT_MM);

  // Set baud rate "Only standard baud rates are supported"
  tfmini.setBaudRate(115200);

  // Disable the device
  tfmini.setEnabled(false);

  // Enable the device
  tfmini.setEnabled(true);

  // Restore to factory settings
  tfmini.restoreFactorySettings();

  // Persist configuration into the device otherwise will be reset with the next
  // power cyle
  tfmini.saveSettings();
}

void loop() {}