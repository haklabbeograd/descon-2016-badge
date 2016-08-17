/**
 * Badge Control
 * Arduino code that give's the ESP8266 control over the board over
 * serial interface. ESP can send comands to the LCD and play tones on
 * the buzzer, the Arduino regualrly scans the joystick and buttons and 
 * send back satus messages. 
 * 
 * Protocol:
 * The protocol is simple framing with CRC, that is somwhat customised
 * to save buffer space. There are two types of incoming messages from 
 * the ESP, control messages, and LCD frame buffer messages, they are 
 * diferentiated by different Start Bytes STX and LCDSTX, and a 
 * single type of outgoing status messages, all are framed as folows.
 * 
 * [Start byte][Escaped[Data Array]][End Byte]
 * 
 * If the Data Array has any bytes that are either the Start Byte,
 * End Byte, or the Escape Byte, the byte is presceded by  the Escape
 * Byte. Example: 
 * [Data] = [D1][D2][D3][D4 = Start Byte][D5].
 * [Escaped[Data]] = [D1][D2][D3][Escape Byte][D4 = Start Byte][D5]
 * 
 * Control Messages ESP->Arduino
 * [Data] <= [ID][CMD][CMD Data][CRC]
 * [Tone MSG] <= [ID][Tone CMD][FreqH][FreqL][DurH][DurL][CRC]
 * [LCD Control] <= [ID][LCDC CMD][Control Byte][CRC]
 * ID = Message ID
 * CMD = What command the message refers to. (Tone and LCD control msg)
 * CRC = Single byte CRC of the ID,CMD, and CMD Data, byte array.
 * FreqH/L = High and low byte od the frequency 16bit integer
 * DurH/L = High and low byte of the duration 16bit integer
 * Control Byte = Byte to be sent to the LCD as a control byte.
 * 
 * LCD Frame Buffer Messages ESP->Arduino
 * [Data] <= [LCD Frame Buffer]
 * This type of message doen't use ID CMD or the CRC bytes, all bytes
 * between the Start and End Bytes are sent to the LCD as Data bytes. 
 * Maximum 504 bytes of frame buffer is possible.
 * 
 * Status Messages
 * [Data] <= [JXH][JXL][JYH][JYL][JB][B1][B2][EN]<[ID][ACK]>[CRC]
 * JXH/L = Joystick X axis 16bit status
 * JYH/L = Joystick Y axis 16bit status
 * JB = Joystick Button status
 * B1 = Button 1 status
 * B2 = Button 2 status
 * EN = Receved message enable, if a message was receved this is set to
 * 		1 if the message was a control message, and 2 if it was a frame
 * ID = ID of the message that was receved, only sent if EN is set
 * ACK = 1 if message was receved corectly, 0 otherwise. only sent EN==1
 * @author Filip Dulic
 * @version 1.0 18/8/2016 
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "Messanger.h"
#include "LCD.h"
#include "StatusMsg.h"
#include "common.h"


SoftwareSerial sSerial(7, 6); //RX,TX

void setup() {
  sSerial.begin(9600);
  LCD.begin();
}

void loop() {
	while (sSerial.available()) {
		byte read = sSerial.read();
		if (Messanger.receveByte(read) == true) break;
	}
	
	if (Messanger.MessageReceved() == true) {
		if (Messanger.handleMessage() == true) {
			Status.setReturnMsgACK(1); //ACK
		} else {
			Status.setReturnMsgACK(0); //NOACK
		}
		Status.setReturnMsgEN(1);
		Status.setReturnMsgID(Messanger.getMessageID());
	}
	
	if(LCD.BufferReceved() == true) {
		LCD.sendBuffer();
		Status.setReturnMsgEN(2);  //EN 2 fro LCD buffer
		Status.setReturnMsgID(0);  //ID=0
		Status.setReturnMsgACK(1); //ACK
		LCD.clearBuffer();
	}
	
	//Status.send();
}
