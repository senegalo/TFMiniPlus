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

#include "TFMiniPlus.h"
#include "TFMiniPlusConstants.h"

template <std::size_t N>
constexpr std::size_t buffer_size(const uint8_t (&buffer)[N]) noexcept {
  return N;
}

void TFMiniPlus::begin(Stream* serial) { _stream = serial; }

bool TFMiniPlus::readData() {
  uint8_t buffer[DATA_FRAME_LENGTH];

  TFMiniPlus::resetBuffer(_readDataBuffer, buffer_size(_readDataBuffer));

  // skip to the first data frame header
  TFMiniPlus::skipToFrameHeader(DATA_FRAME_MARKER);

  // get the data body "all - frame marker byte"
  _stream->readBytes(&buffer[1], DATA_FRAME_LENGTH - 1);

  // reconstruct the frame for checksum
  buffer[0] = DATA_FRAME_MARKER;

  // validate data frame
  if (TFMiniPlus::validateChecksum(buffer, DATA_FRAME_LENGTH)) {
    TFMiniPlus::copyBuffer(_readDataBuffer, buffer, buffer_size(_readDataBuffer), DATA_FRAME_LENGTH);
    return true;
  }

  return false;
}

uint16_t TFMiniPlus::getDistance() {
  return uint16_t(_readDataBuffer[2]) | (uint16_t(_readDataBuffer[3]) << 8);
}

uint16_t TFMiniPlus::getSensorRawTemperature() {
  return uint16_t(_readDataBuffer[6]) | (uint16_t(_readDataBuffer[7]) << 8);
}

double TFMiniPlus::getSensorTemperature() {
  return TFMiniPlus::getSensorRawTemperature() / 8.0 - 256;
}

uint16_t TFMiniPlus::getSignalStrength() {
  return uint16_t(_readDataBuffer[4]) | (uint16_t(_readDataBuffer[5]) << 8);
}

String TFMiniPlus::getVersion() {
  uint8_t commandResponse[7];

  TFMiniPlus::write(GetVersionCommand, buffer_size(GetVersionCommand));
  if (!TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse))) {
    return "Error";
  }
  return String(commandResponse[5]) + "." + String(commandResponse[4]) + "." +
         String(commandResponse[3]);
}

bool TFMiniPlus::systemReset() {
  uint8_t commandResponse[5];
  TFMiniPlus::write(ResetCommand, buffer_size(ResetCommand));
  if (TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse))
    && commandResponse[3] == 0x00) {
    return true;
  }
  return false;
}

bool TFMiniPlus::setFrameRate(uint16_t framerate) {
  uint8_t newRateHight = (uint8_t)(framerate >> 8);
  uint8_t newRateLow = (uint8_t)framerate;
  uint8_t buffer[6] = {CMD_FRAME_MARKER, 0x06, 0x03, newRateLow, newRateHight};
  uint8_t commandResponse[6];

  constexpr size_t commandBufferSize = buffer_size(buffer);
  buffer[commandBufferSize-1] = TFMiniPlus::generateChecksum(buffer, commandBufferSize-1);

  TFMiniPlus::write(buffer, commandBufferSize);
  if (TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse)) &&
      TFMiniPlus::readInt16FromBuffer(commandResponse, 3) ==
          TFMiniPlus::readInt16FromBuffer(buffer, 3)) {
    return true;
  }

  return false;
}

void TFMiniPlus::triggerDetection() {
  TFMiniPlus::write(TriggerDetectionCommand, buffer_size(TriggerDetectionCommand));
}

bool TFMiniPlus::setMeasurementTo(uint16_t measurement) {
  uint8_t commandResponse[5];
  uint8_t measurementH = (uint8_t)(measurement >> 8);
  uint8_t measurementL = (uint8_t)measurement;
  uint8_t buffer[5] = {CMD_FRAME_MARKER, 0X05, 0x05, measurementH, measurementL};

  TFMiniPlus::write(buffer, buffer_size(buffer));
  if (TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse)) &&
      TFMiniPlus::readInt16FromBuffer(commandResponse, 3) ==
          TFMiniPlus::readInt16FromBuffer(buffer, 3)) {
    return true;
  }

  return false;
}

bool TFMiniPlus::setBaudRate(uint32_t baud) {
  uint8_t commandResponse[8];
  uint8_t baud_b4 = (uint8_t)(baud >> 24);
  uint8_t baud_b3 = (uint8_t)(baud >> 16);
  uint8_t baud_b2 = (uint8_t)(baud >> 8);
  uint8_t baud_b1 = (uint8_t)baud;
  uint8_t buffer[8] = {CMD_FRAME_MARKER, 0x08, 0x06, baud_b1, baud_b2, baud_b3, baud_b4};
  buffer[buffer_size(buffer)-1] = TFMiniPlus::generateChecksum(buffer, buffer_size(buffer)-1);
  TFMiniPlus::write(buffer, buffer_size(buffer)-1);
  if (TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse)) &&
      TFMiniPlus::readInt32FromBuffer(commandResponse, 3) ==
          TFMiniPlus::readInt32FromBuffer(buffer, 3)) {
    return true;
  }
  return false;
}

bool TFMiniPlus::setEnabled(bool state) {
  uint8_t commandResponse[5];
  uint8_t buffer[5] = {CMD_FRAME_MARKER, 0x05, 0x07};
  if (state) {
    buffer[3] = 0x00;
    buffer[4] = 0x66;
  } else {
    buffer[3] = 0x01;
    buffer[4] = 0x67;
  }
  TFMiniPlus::write(buffer, 5);
  if (TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse)) &&
      TFMiniPlus::readInt16FromBuffer(commandResponse, 3) ==
          TFMiniPlus::readInt16FromBuffer(buffer, 3)) {
    return true;
  }
  return false;
}

bool TFMiniPlus::restoreFactorySettings() {
  uint8_t commandResponse[4];
  TFMiniPlus::write(RestoreFactorySettingsCommand, buffer_size(RestoreFactorySettingsCommand));
  if (TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse)) &&
      commandResponse[3] == 0x00) {
    return true;
  }
  return false;
}

bool TFMiniPlus::saveSettings() {
  // 5A 04 11 6F
  uint8_t commandResponse[4];
  TFMiniPlus::write(SaveSettingsCommand, buffer_size(SaveSettingsCommand));
  if (TFMiniPlus::readCommandResponse(commandResponse, buffer_size(commandResponse)) &&
      commandResponse[3] == 0x00) {
    return true;
  }
  return false;
}

TFMiniPlus::TFMiniPlus() {}

bool TFMiniPlus::validateChecksum(const uint8_t dataBuffer[], size_t length) {
  uint8_t sum = TFMiniPlus::generateChecksum(dataBuffer, length - 1);
  uint8_t checksum = dataBuffer[length - 1];
  if (sum == checksum) {
    return true;
  } else {
    return false;
  }
}

void TFMiniPlus::write(const uint8_t buffer[], size_t length) {
  for (uint8_t i = 0; i < length; i++) {
    _stream->write(buffer[i]);
  }
}

bool TFMiniPlus::readCommandResponse(uint8_t buffer[], size_t buffer_size) {
  // skip to the first command frame header
  TFMiniPlus::skipToFrameHeader(CMD_FRAME_MARKER);

  // get the body length
  uint8_t length = 0x00;
  _stream->readBytes(&length, 1);

  // get the command body "all - (header and length bytes)"
  uint8_t response[MAX_CMD_RESPONSE_LENGTH] = {CMD_FRAME_MARKER, length};
  _stream->readBytes(&response[2], length - 2);

  // validate command frame
  if (TFMiniPlus::validateChecksum(response, length)) {
    TFMiniPlus::copyBuffer(buffer, response, buffer_size, length);
    return true;
  }

  return false;
}

uint8_t TFMiniPlus::generateChecksum(const uint8_t buffer[], size_t length) {
  uint16_t sum = 0x0000;
  for (uint8_t i = 0; i < length; i++) {
    sum += buffer[i];
  }

  return (uint8_t)sum;
}

void TFMiniPlus::skipToFrameHeader(uint8_t frameHeader) {
  uint8_t currentChar = 0x00;
  while (currentChar != frameHeader) {
    _stream->readBytes(&currentChar, 1);
  }
}

uint16_t TFMiniPlus::readInt16FromBuffer(const uint8_t buffer[], size_t startIndex) {
  return (uint16_t(buffer[startIndex]) << 8) | uint16_t(buffer[startIndex + 1]);
}

uint32_t TFMiniPlus::readInt32FromBuffer(const uint8_t buffer[], size_t startIndex) {
  return (uint32_t(buffer[startIndex]) << 24) | (uint32_t(buffer[startIndex + 1]) << 16) |
    (uint32_t(buffer[startIndex + 2]) << 8) | uint32_t(buffer[startIndex + 3]);
}

void TFMiniPlus::copyBuffer(uint8_t buffer1[], const uint8_t buffer2[], size_t buffer_size,
                            size_t length) {
  for (uint8_t i = 0; i < length && i < buffer_size; i++) {
    buffer1[i] = buffer2[i];
  }
}

void TFMiniPlus::resetBuffer(uint8_t buffer[], size_t length) {
  for (uint8_t i = 0; i < length; i++) {
    buffer[i] = 0x00;
  }
}