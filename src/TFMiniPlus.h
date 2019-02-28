/*
 * TFMiniPlus.h
 *
 *  Created on: Feb 9, 2019
 *      Author: Karim Mansour
 * An Arduino driver for the TFMiniPlus Lidar System
 *
 * Released under the GPL-3.0
 *
 * https://github.com/senegalo/tfmini-plus
 */

#ifndef TFMINIPLUS_H_
#define TFMINIPLUS_H_

#include <cstddef>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define TFMINI_MEASUREMENT_CM 0x0165
#define TFMINI_MEASUREMENT_MM 0x066A

using namespace std;

class TFMiniPlus {
 public:
  TFMiniPlus();
  void begin(Stream* serial);
  bool readData();
  uint16_t getDistance();
  double_t getSensorTemperature();
  uint16_t getSensorRawTemperature();
  uint16_t getSignalStrength();
  String getVersion();
  bool systemReset();
  bool setFrameRate(uint16_t rate);
  void triggerDetection();
  bool setMeasurementTo(uint16_t measurment);
  bool setBaudRate(uint32_t baud);
  bool setEnabled(bool state);
  bool restoreFactorySettings();
  bool saveSettings();

 private:
  uint8_t _readDataBuffer[9];
  Stream* _stream;
  void write(const uint8_t buffer[], const size_t length);
  bool readCommandResponse(uint8_t buffer[], size_t buffer_size);
  void skipToFrameHeader(uint8_t farameHeader);

  static bool validateChecksum(const uint8_t buffer[], const size_t length);
  static uint8_t generateChecksum(const uint8_t buffer[], size_t length);
  static void resetBuffer(uint8_t buffer[], size_t length);
  static void copyBuffer(uint8_t buffer1[], const uint8_t buffer2[], size_t buffer_size, size_t length);
  static uint16_t readInt16FromBuffer(const uint8_t buffer[], size_t startIndex);
  static uint32_t readInt32FromBuffer(const uint8_t buffer[], size_t startIndex);
};

#endif