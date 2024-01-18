//------libraries------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"
#include <virtuabotixRTC.h>
#include <Time.h>
#include "time.h"
#include "WiFi.h"
#include <dht11.h>
#include <Preferences.h>

//------define------
#define CLK 20
#define DT 19
#define SW 26
#define BT1 48
#define BT2 47
#define BT3 33
#define DHT11PIN 6
Adafruit_SSD1306 display1(128, 64, &Wire, 21, 500000UL);
Adafruit_SSD1306 display2(128, 64, &Wire1, -1, 400000UL);
//encoder and buttons
int current_screen = 0, currentStateCLK, lastStateCLK, first = 0, setting = 0, settings = 0, settings1 = 0, settings_time, bezelPos = 0, lastEstate, btnStateE, time_chosed_btn;
unsigned long lastButtonPress = 0;




//calculator
int calcPos = 0;
char operators[][2] = {{"0"}, {"1"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"+"}, {"-"}, {"/"}, {"*"}, {"R"}, {"."}, {"="}, {"C"}};
String operation = "";

void setup() {
  Serial.begin(9600);

  //------PINMODES------
  pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);

  //------DISPLAY'S SETUP-------
  //built-in display
  Wire.begin(17,18);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display1.clearDisplay();
  display1.display();
  
  //second display
  Wire1.begin(41,42);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display2.clearDisplay();
  display2.display();

  display2.setTextSize(1);
  display2.setTextColor(WHITE); 
  display2.setCursor(0, 0);
}

void loop(){
  calculator();
}

void calculator(){
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
      calcPos--;
      if(calcPos < 0) calcPos = 17;
    }
    else {
      calcPos++;
      if(calcPos > 17) calcPos = 0;
    }

    operation = String(operators[calcPos][0]);
    Serial.println(operation);
    display2.clearDisplay();
    display2.setCursor(0, 0);
    display2.print(operation);
    display2.display();
  }
  lastStateCLK = currentStateCLK;
  delay(1);  
}

