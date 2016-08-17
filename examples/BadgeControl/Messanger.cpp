#include "Messanger.h"
#include "common.h"


MessangerClass Messanger;

MessangerClass::MessangerClass() {
	startByte = NOSTART;
	rxBufferIndex=0;
	msgReceved=false;
	bufferOverflow=false;
}

bool MessangerClass::checkCRC() {
	byte CRC = 0;
	byte ID = msgRecevedBuffer[0];
	byte Len;
	if (ID == LCDC) Len = 3;
	if (ID == TONE) Len = 6;
	
	for (int i = 0;i<Len;i++) {
		CRC += msgRecevedBuffer[i];
	}
	return CRC == msgRecevedBuffer[Len];
}

bool MessangerClass::checkEscape() {
	if (escNext == false) {
		if (read == ESC) {
			escNext = true;
			return true;
		}
	} else {
		escaped = true;
		escNext = false;
	}
	return false;
}

bool MessangerClass::handleStartByte() {
	if (read == STX || read == LCDSTX) {
		startByte = read;
		if (startByte == STX) {
			rxBufferIndex = 0;
			bufferOverflow = false;
		}
		if (startByte == LCDSTX) {
			LCD.clearBuffer();
		}
		return true;
	}
	return false;
}

bool MessangerClass::handleEndByte() {
	if (read == ETX) {
		if (startByte == STX) {
			for (byte i = 0; i < rxBufferIndex; i++)
				msgRecevedBuffer[i] = rxBuffer[i];
			rxBufferIndex = 0;
			bufferOverflow = false;
			msgReceved = true;
			ID = msgRecevedBuffer[0];
			CMD = msgRecevedBuffer[1];
		}
		if (startByte == LCDSTX){
			LCD.setRecevedBuffer();
		}
		startByte = NOSTART;
		return true;
	}
	return false;
}

bool MessangerClass::receveByte(byte bRead) {
	escaped = false;
	read = bRead;
	if (checkEscape() == true) return false;
	if (escaped == false) {
		if (handleStartByte() == true) return false;
		if (handleEndByte() == true) return true;
	}
	if (startByte == LCDSTX) {
			LCD.addToBuffer(read);
			return false;
		}
	if (startByte == STX) {
		rxBuffer[rxBufferIndex++] = read;
	}
	if (rxBufferIndex == RX_BUFFER_MAX){
		//fuckup buffer overload
		rxBufferIndex = 0;
		startByte = NOSTART;
		bufferOverflow = true;
		return true;
	}
	return false;
}

bool MessangerClass::handleMessage() {
	msgReceved = false;
	switch (CMD)
	{
	case LCDC:
		if (checkCRC() == true) {
			//send LCD ctl..
			if (msgRecevedBuffer[2] == 0)
				LCD.reset();
			else
				LCD.sendCommand(msgRecevedBuffer[2]);
			return true;
		} else return false;
		break;
	case TONE:
		if (checkCRC() == true) {
			int freq = (msgRecevedBuffer[2] << 8 | msgRecevedBuffer[3]);
			int dur  = (msgRecevedBuffer[4] << 8 | msgRecevedBuffer[5]);
			LCD.pause();
			tone(12,freq,dur);
			delay(dur);
			LCD.unPause(); 
			return true;
		} else return false;
		break;
	default:
		return false;
	}
}
