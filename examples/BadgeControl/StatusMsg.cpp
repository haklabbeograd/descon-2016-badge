#include <Arduino.h>
#include "StatusMsg.h"

StatusMsg Status;

StatusMsg::StatusMsg() {
	pinMode(JoyButtPin, INPUT);
	pinMode(Butt1Pin, INPUT);
	pinMode(Butt2Pin, INPUT);
	bufferIndex = 0;
}

void StatusMsg::updateCRC() {
	CRC = 0;
	CRC += joyXHigh;
	CRC += joyXLow;
	CRC += joyYHigh;
	CRC += joyYLow;
	CRC += joyButtState;
	CRC += butt1State;
	CRC += butt2State;
	CRC += returnMsgEN;
	if (returnMsgEN == 1) {
		CRC += returnMsgID;
		CRC += returnMsgACK;
	}
}

void StatusMsg::updateStatus() {
	int joyX = analogRead(JoyXPin);
	joyXHigh = joyX >> 8;
	joyXLow  = joyX & 0xFF;
	
	int joyY = analogRead(JoyYPin);
	joyYHigh = joyY >> 8;
	joyYLow  = joyY & 0xFF;
	
	joyButtState = digitalRead(JoyButtPin);
	butt1State = digitalRead(Butt1Pin);
	butt2State = digitalRead(Butt2Pin);
	updateCRC();
}

void StatusMsg::addEscapedData(byte data) {
	if ((data == STX)||(data == LCDSTX)||(data == ESC)||(data == ETX))
		buffer[bufferIndex++] = ESC;
	buffer[bufferIndex++] = data;
}

void StatusMsg::compileStatusMsg() {
	buffer[0] = STX;
	bufferIndex = 1;
	addEscapedData(joyXHigh);
	addEscapedData(joyXLow);
	addEscapedData(joyYHigh);
	addEscapedData(joyYLow);
	addEscapedData(joyButtState);
	addEscapedData(butt1State);
	addEscapedData(butt2State);
	addEscapedData(returnMsgEN);
	if (returnMsgEN	== 1) {
		addEscapedData(returnMsgID);
		addEscapedData(returnMsgACK);
	}
	addEscapedData(CRC);
	buffer[bufferIndex++] = ETX;
}

void StatusMsg::send() {
	updateStatus();
	compileStatusMsg();
	for(byte i; i < bufferIndex; i++)
		sSerial.write(buffer[i]);
	returnMsgEN = 0;
}
