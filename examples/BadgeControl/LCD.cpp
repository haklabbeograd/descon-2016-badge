#include "LCD.h"

LCDClass LCD;

#include <Arduino.h>

/* Pin definitions:
Most of these pins can be moved to any digital or analog pin.
DN(MOSI)and SCLK should be left where they are (SPI pins). The
LED (backlight) pin should remain on a PWM-capable pin. */
const int scePin 	= 8;	// SCE
const int rstPin 	= 9;	// RST
const int dcPin 	= 10;	// DC.
const int sdinPin 	= 11;	// DN
const int sclkPin 	= 13;	// SCLK

/* PCD8544-specific defines: */
#define LCD_COMMAND  0
#define LCD_DATA     1

/* 84x48 LCD Defines: */
#define LCD_WIDTH   84 // Note: x-coordinates go wide
#define LCD_HEIGHT  48 // Note: y-coordinates go high
#define WHITE       0  // For drawing pixels. A 0 draws white.
#define BLACK       1  // A 1 draws black.

void LCDClass::sendCommand(byte command) {
  //Tell the LCD that we are writing either to data or a command
  digitalWrite(dcPin, LCD_COMMAND);
  //Send the data
  digitalWrite(scePin, LOW);
  SPI.transfer(command); //shiftOut(sdinPin, sclkPin, MSBFIRST, data);
  digitalWrite(scePin, HIGH);
}

void LCDClass::sendBuffer() {
  //Tell the LCD that we are writing either to data or a command
  digitalWrite(dcPin, LCD_DATA);

  //Send the data
  digitalWrite(scePin, LOW);
  SPI.transfer(buffer,index);
  digitalWrite(scePin, HIGH);
}

void LCDClass::reset() {
  digitalWrite(rstPin, LOW);
  digitalWrite(rstPin, HIGH);
}

//This sends the magical commands to the PCD8544
void LCDClass::begin() {
  //Configure control pins
  pinMode(scePin, OUTPUT);
  pinMode(rstPin, OUTPUT);
  pinMode(dcPin, OUTPUT);
  pinMode(sdinPin, OUTPUT);
  pinMode(sclkPin, OUTPUT);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  //Reset the LCD to a known state
  reset();

  sendCommand(0x21); //Tell LCD extended commands follow
  sendCommand(0xA0); //Set LCD Vop (Contrast)
  sendCommand(0x04); //Set Temp coefficent
  sendCommand(0x16); //LCD bias mode 1:48 (try 0x13)
  //We must send 0x20 before modifying the display control mode
  sendCommand(0x20);
  sendCommand(0x0C); //Set display control, normal mode.
}

