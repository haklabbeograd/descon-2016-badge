#ifndef _MESSANGER_H_INCLUDED
#define _MESSANGER_H_INCLUDED

#include "LCD.h"

#define NOSTART 0
#define TONE 1
#define LCDC 2

#define STX		1
#define ETX 	2
#define ESC 	3
#define LCDSTX  4

#define RX_BUFFER_MAX 16

class MessangerClass {
public:
	MessangerClass();
	
	bool receveByte(byte read);
	bool checkCRC();
	bool MessageReceved() {return msgReceved;}
	byte getMessageID() {return ID;}
	byte getMessageCMD() {return CMD;}
	byte *getMessage() {return msgRecevedBuffer;}
	bool handleMessage();
private:
	bool checkEscape();
	bool handleStartByte();
	bool handleEndByte();
	byte read;
	byte startByte;
	byte rxBuffer[RX_BUFFER_MAX];
	byte rxBufferIndex;
	byte msgRecevedBuffer[RX_BUFFER_MAX];
	bool msgReceved;
	bool bufferOverflow;
	bool escaped;
	bool escNext;
	byte ID;
	byte CMD;
	
};

extern MessangerClass Messanger;
#endif

