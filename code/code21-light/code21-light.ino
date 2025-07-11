//------libraries------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <virtuabotixRTC.h>
#include <Time.h>
#include "time.h"
#include "WiFi.h"
#include <dht11.h>
#include <Preferences.h>

Adafruit_SSD1306 display1(128, 64, &Wire, 21, 500000UL);

void setup() {
  Serial.begin(9600);

  //------DISPLAY'S SETUP-------
  //built-in display
  Wire.begin(17,18);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display1.clearDisplay();
  display1.display();
}

void loop() {
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setTextColor(WHITE); 
  display1.setCursor(0, 0);
  float voltage = analogRead(1);
  Serial.println(voltage);
  float voltagee = 0.204*voltage;
  Serial.println(voltagee);

  display1.setCursor(0,0);
  display1.print(voltagee);
  display1.display();
  delay(200);

}
