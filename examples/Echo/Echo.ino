/*
 * PCD8544 - Interface with Philips PCD8544 (or compatible) LCDs.
 *
 * Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * To use this sketch, connect the eight pins from your LCD like thus:
 *
 * Pin 1 -> +3.3V (rightmost, when facing the display head-on)
 * Pin 2 -> Arduino digital pin 3
 * Pin 3 -> Arduino digital pin 4
 * Pin 4 -> Arduino digital pin 5
 * Pin 5 -> Arduino digital pin 7
 * Pin 6 -> Ground
 * Pin 7 -> 10uF capacitor -> Ground
 * Pin 8 -> Arduino digital pin 6
 *
 * Since these LCDs are +3.3V devices, you have to add extra components to
 * connect it to the digital pins of the Arduino (not necessary if you are
 * using a 3.3V variant of the Arduino, such as Sparkfun's Arduino Pro).
 */


#include <PCD8544.h>
#include <SoftwareSerial.h>

static PCD8544 lcd;

SoftwareSerial sSerial(7, 6);

void setup() {
  lcd.begin(84, 48);
  sSerial.begin(9600);
  lcd.print("DESCON 2016");
  lcd.setCursor(0, 1);
  lcd.print("Badger");
  delay(2000);
}
String sAddr;
String sBuffer;
bool start = false;
void loop() {
  
  
  if(sSerial.available()) {
    tone(12,4100,500);
    sBuffer = sSerial.readString();
    
    if(sBuffer.substring(0,9) == "Ip Addr: " && start==false){
      sAddr = sBuffer.substring(10);
      lcd.setCursor(0, 3);
      lcd.print(sAddr);
      start = true;
    }
    else{
      lcd.clear();
      if(sAddr.length()!=0){
        lcd.setCursor(0, 0);
        lcd.print(sAddr);
      }
      lcd.setCursor(0, 1);
      lcd.print(sBuffer);
    } 
  }
  delay(200);
}

