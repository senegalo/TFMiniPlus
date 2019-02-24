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

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define TFMINI_MEASUREMENT_CM 0x0165
#define TFMINI_MEASUREMENT_MM 0x066A

class TFMiniPlus {
 public:
  TFMiniPlus();
  void begin(Stream* serial);
  bool readData();
  uint16_t getDistance();
  double_t getSensorTempreture();
  uint16_t getSensorRawTempreture();
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
  bool validateChecksum(uint8_t buffer[], uint8_t length);
  uint8_t generateChecksum(uint8_t buffer[], uint8_t length);
  void write(uint8_t buffer[], uint8_t length);
  bool readCommandResponse(uint8_t buffer[]);
  void skipToFrameHeader(uint8_t farameHeader);
  uint16_t readInt16FromBuffer(uint8_t buffer[], uint8_t startIndex);
  uint32_t readInt32FromBuffer(uint8_t buffer[], uint8_t startIndex);
  void copyBuffer(uint8_t buffer1[], uint8_t buffer2[], uint8_t length);
  void resetBuffer(uint8_t buffer[], uint8_t length);
};

#endif