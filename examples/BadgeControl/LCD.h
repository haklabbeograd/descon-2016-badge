#ifndef _LCD_H_INCLUDED
#define _LCD_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>

class LCDClass{
public:
	LCDClass(){index = 0;receved = false;}
	void begin();
	void sendCommand(byte command);
	void sendBuffer();
	void reset();
	void pause(){SPI.end();}
	void unPause(){SPI.begin();}
	void addToBuffer(byte data){if(index < 504)buffer[index++] = data;}
	void clearBuffer(){index=0;receved = false;}
	void setRecevedBuffer(){receved = true;}
	bool BufferReceved(){return receved;}
private:
	byte buffer[504];
	int index;
	bool receved;
};

extern LCDClass LCD;
#endif
