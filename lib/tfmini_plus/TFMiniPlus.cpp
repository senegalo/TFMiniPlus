/*
 * TFMiniPlus.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: senegalo
 */

#include "TFMiniPlus.h"

void TFMiniPlus::begin(Stream* serial) {
	stream = serial;
}

bool TFMiniPlus::readData() {
	uint8_t buffer[9];

	//skip to the first data frame header
	TFMiniPlus::skipToFrameHeader(DATA_FRAME_MARKER);

	//get the data body "all - frame marker byte"
	stream->readBytes(buffer, DATA_FRAME_LENGTH - 1);

	//reconstruct the frame for checksum
	uint8_t frame[DATA_FRAME_LENGTH] = { DATA_FRAME_MARKER };
	for (int i = 1; i < DATA_FRAME_LENGTH; i++) {
		frame[i] = buffer[i - 1];
	}

	//validate data frame
	return TFMiniPlus::validateChecksum(frame, DATA_FRAME_LENGTH);
}

unsigned int TFMiniPlus::getDistance() {
	return (readDataBuffer[2] | (readDataBuffer[3] << 8));
}

unsigned int TFMiniPlus::getSensorTempreture() {
	return (readDataBuffer[6] | (readDataBuffer[7] << 8));
}

unsigned int TFMiniPlus::getSignalStrength() {
	return (readDataBuffer[4] | (readDataBuffer[5] << 8));

}

String TFMiniPlus::getVersion() {
	uint8_t buffer[4] = { CMD_FRAME_MARKER, 0x04, 0x01, 0x5F };
	uint8_t commandResponse[7];
	TFMiniPlus::write(buffer, 4);
	if (!TFMiniPlus::readCommandResponse(commandResponse)) {
		return "Error";
	}
	return "Version " + String(commandResponse[5]) + "."
			+ String(commandResponse[4]) + "." + String(commandResponse[3]);
}

bool TFMiniPlus::systemReset() {
	uint8_t buffer[4] = { CMD_FRAME_MARKER, 0x04, 0x02, 0x60 };
	uint8_t commandResponse[5];
	TFMiniPlus::write(buffer, 4);
	if (TFMiniPlus::readCommandResponse(commandResponse)
			&& commandResponse[4] == 0x60) {
		return true;
	}
	return false;
}

bool TFMiniPlus::setUpdateRate(uint16_t rate) {
	uint8_t newRateHight = (uint8_t) (rate >> 8);
	uint8_t newRateLow = (uint8_t) rate;
	uint8_t buffer[5] =
			{ CMD_FRAME_MARKER, 0x06, 0x03, newRateLow, newRateHight };
	uint8_t commandResponse[5];
	buffer[4] = TFMiniPlus::generateChecksum(buffer, 4);
	TFMiniPlus::write(buffer, 5);
	if (TFMiniPlus::readCommandResponse(commandResponse)
			&& TFMiniPlus::readInt16FromBuffer(commandResponse, 3)
					== TFMiniPlus::readInt16FromBuffer(buffer, 3)) {
		return true;
	}
	return false;
}

void TFMiniPlus::triggerDetection() {
	uint8_t buffer[4] = { CMD_FRAME_MARKER, 0X04, 0x04, 0x62 };
	TFMiniPlus::write(buffer, 4);
}

bool TFMiniPlus::setMeasurementTo(uint16_t measurement) {
	uint8_t commandResponse[5];
	uint8_t measurementH = (uint8_t) (measurement >> 8);
	uint8_t measurementL = (uint8_t) measurement;
	uint8_t buffer[5] = { CMD_FRAME_MARKER, 0X05, 0x05, measurementH,
			measurementL };
	TFMiniPlus::write(buffer, 5);
	if (TFMiniPlus::readCommandResponse(commandResponse)
			&& TFMiniPlus::readInt16FromBuffer(commandResponse, 3)
					== TFMiniPlus::readInt16FromBuffer(buffer, 3)) {
		return true;
	}
	return false;
}

bool TFMiniPlus::setBaudRate(uint32_t baud) {
	uint8_t commandResponse[8];
	uint8_t baud_b1 = (uint8_t) (baud >> 24);
	uint8_t baud_b2 = (uint8_t) (baud >> 16);
	uint8_t baud_b3 = (uint8_t) (baud >> 8);
	uint8_t baud_b4 = (uint8_t) baud;
	uint8_t buffer[8] = { CMD_FRAME_MARKER, 0x08, 0x06, baud_b4, baud_b3,
			baud_b2, baud_b1 };
	buffer[7] = TFMiniPlus::generateChecksum(buffer, 7);
	TFMiniPlus::write(buffer, 8);
	if (TFMiniPlus::readCommandResponse(commandResponse)
			&& TFMiniPlus::readInt32FromBuffer(commandResponse, 3)
					== TFMiniPlus::readInt32FromBuffer(buffer, 3)) {
		return true;
	}
	return false;
}

bool TFMiniPlus::setEnabled(uint16_t state) {
	uint8_t commandResponse[5];
	uint8_t stateL = (uint8_t) state;
	uint8_t stateH = (uint8_t) (state >> 8);
	uint8_t buffer[5] = { CMD_FRAME_MARKER, 0x05, 0x07, stateH, stateL };
	TFMiniPlus::write(buffer, 5);
	if (TFMiniPlus::readCommandResponse(commandResponse)
			&& TFMiniPlus::readInt16FromBuffer(commandResponse, 3)
					== TFMiniPlus::readInt16FromBuffer(buffer, 3)) {
		return true;
	}
	return false;
}

bool TFMiniPlus::restoreFactorySettings() {
	uint8_t commandResponse[4];
	uint8_t buffer[4] = { CMD_FRAME_MARKER, 0x04, 0x10, 0x6E };
	TFMiniPlus::write(buffer, 4);
	if (TFMiniPlus::readCommandResponse(commandResponse)
			&& TFMiniPlus::readInt16FromBuffer(commandResponse, 2)
					== TFMiniPlus::readInt16FromBuffer(buffer, 2)) {
		return true;
	}
	return false;
}

bool TFMiniPlus::saveSettings() {
	//5A 04 11 6F
	uint8_t commandResponse[4];
	uint8_t buffer[4] = { CMD_FRAME_MARKER, 0x04, 0x11, 0x6F };
	TFMiniPlus::write(buffer, 4);
	if (TFMiniPlus::readCommandResponse(commandResponse)
			&& TFMiniPlus::readInt16FromBuffer(commandResponse, 2)
					== TFMiniPlus::readInt16FromBuffer(buffer, 2)) {
		return true;
	}
	return false;
}

TFMiniPlus::TFMiniPlus() {}

bool TFMiniPlus::validateChecksum(uint8_t dataBuffer[], int length) {
	uint8_t sum = TFMiniPlus::generateChecksum(dataBuffer, length - 2);
	uint8_t checksum = dataBuffer[length - 1];
	if (sum == checksum) {
		return true;
	} else {
		return false;
	}
}

void TFMiniPlus::write(uint8_t buffer[], int length) {
	for (int i = 0; i < length; i++) {
		stream->write(buffer[i]);
	}
}

bool TFMiniPlus::readCommandResponse(uint8_t buffer[]) {
	//skip to the first command frame header
	TFMiniPlus::skipToFrameHeader(CMD_FRAME_MARKER);

	//get the body length
	uint8_t length = 0x00;
	stream->readBytes(&length, 1);

	//get the command body "all - (header and length bytes)"
	stream->readBytes(buffer, length - 2);

	//reconstruct the frame for checksum
	uint8_t frame[MAX_CMD_RESPONSE_LENGTH] = { CMD_FRAME_MARKER, length };
	for (int i = 2; i < length; i++) {
		frame[i] = buffer[i - 2];
	}

	//validate command frame
	return TFMiniPlus::validateChecksum(frame, length);
}

uint8_t TFMiniPlus::generateChecksum(uint8_t buffer[], int length) {
	uint8_t sum = 0x00;
	for (int i = 0; i < length; i++) {
		sum += buffer[i];
	}
	return sum;
}

void TFMiniPlus::skipToFrameHeader(uint8_t frameHeader) {
	uint8_t currentChar = 0x00;
	while (currentChar != frameHeader) {
		stream->readBytes(&currentChar, 1);
	}
}

uint16_t TFMiniPlus::readInt16FromBuffer(uint8_t buffer[], uint8_t startIndex) {
	return ((uint16_t) (0x0000)) | buffer[startIndex] << 8
			| buffer[startIndex + 1];
}

uint32_t TFMiniPlus::readInt32FromBuffer(uint8_t buffer[], uint8_t startIndex) {
	return ((uint32_t) (0x00000000)) | buffer[startIndex] << 24
			| buffer[startIndex + 1] << 16 | buffer[startIndex + 2] << 8
			| buffer[startIndex + 3];
}
