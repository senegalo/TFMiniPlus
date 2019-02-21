/*
 * TFMiniPlus.h
 *
 *  Created on: Feb 9, 2019
 *      Author: senegalo
 */

#ifndef TFMINIPLUS_H_
#define TFMINIPLUS_H_
#endif /* TFMINIPLUS_H_ */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define DATA_FRAME_MARKER 0x59
#define DATA_FRAME_LENGTH 9
#define CMD_FRAME_MARKER 0x5A
#define MAX_CMD_RESPONSE_LENGTH 8
#define MEASUREMENT_CM 0x0165
#define MEASUREMENT_MM 0x066A
#define ENABLED 0x0066
#define DISABLED 0x0167

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
  bool setUpdateRate(uint16_t rate);
  void triggerDetection();
  bool setMeasurementTo(uint16_t measurment);
  bool setBaudRate(uint32_t baud);
  bool setEnabled(uint16_t state);
  bool restoreFactorySettings();
  bool saveSettings();

 private:
  uint8_t readDataBuffer[9];
  Stream* stream;
  bool validateChecksum(uint8_t buffer[], uint8_t length);
  uint8_t generateChecksum(uint8_t buffer[], uint8_t length);
  void write(uint8_t buffer[], uint8_t length);
  bool readCommandResponse(uint8_t buffer[]);
  void skipToFrameHeader(uint8_t farameHeader);
  uint16_t readInt16FromBuffer(uint8_t buffer[], uint8_t startIndex);
  uint32_t readInt32FromBuffer(uint8_t buffer[], uint8_t startIndex);
  void printBuffer(uint8_t buffer[], uint8_t length);
  void copyBuffer(uint8_t buffer1[], uint8_t buffer2[], uint8_t length);
  void resetBuffer(uint8_t buffer[], uint8_t length);
};