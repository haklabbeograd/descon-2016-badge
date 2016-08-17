#ifndef _STATUSMSG_H_INCLUDED
#define _STATUSMSG_H_INCLUDED
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "common.h"

#define STATUS_BUFF_MAX 22
#define JoyButtPin 4
#define Butt1Pin 3
#define Butt2Pin 2
#define JoyXPin 1
#define JoyYPin 0

class StatusMsg {
public:
	StatusMsg();
	void send();
	void setReturnMsgEN(byte EN) {returnMsgEN = EN;}
	void setReturnMsgID(byte ID) {returnMsgID = ID;}
	void setReturnMsgACK(byte ACK) {returnMsgACK = ACK;} 
private:
	void updateStatus();
	void compileStatusMsg();
	void updateCRC();
	void addEscapedData(byte data); 
	byte buffer[STATUS_BUFF_MAX];
	byte bufferIndex;
	byte CRC;
	byte joyXHigh;
	byte joyXLow;
	byte joyYHigh;
	byte joyYLow;
	byte butt1State;
	byte butt2State;
	byte joyButtState;
	byte returnMsgEN;
	byte returnMsgID;
	byte returnMsgACK;
};
extern StatusMsg Status;
extern SoftwareSerial sSerial;

#endif
