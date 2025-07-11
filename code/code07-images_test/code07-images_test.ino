#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"

Adafruit_SSD1306 display1(128, 64, &Wire, 21, 500000UL);

void setup() 
{
  Wire.begin(17,18);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display1.clearDisplay();
  display1.display();

}

void loop() 
{
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_crying, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_blinking, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_confused, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_cuppa, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_fuckU, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_love, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_music1, 128, 64, WHITE);
  display1.display();
  delay(300);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_music2, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_normal, 128, 64, WHITE);
  display1.display();
  delay(1000);
  
  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_sad, 128, 64, WHITE);
  display1.display();
  delay(1000);

  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_sleeping1, 128, 64, WHITE);
  display1.display();
  delay(300);

  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_sleeping2, 128, 64, WHITE);
  display1.display();
  delay(1000);

  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_swearing, 128, 64, WHITE);
  display1.display();
  delay(1000);

  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_thinking, 128, 64, WHITE);
  display1.display();
  delay(1000);

  display1.clearDisplay();
  display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
  display1.display();
  delay(1000);
}