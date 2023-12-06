




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

//------define------
#define CLK 20
#define DT 19
#define SW 26
#define BT1 48
#define BT2 47
#define BT3 33

//------Displays init------
Adafruit_SSD1306 display1(128, 64, &Wire, 21, 500000UL);
Adafruit_SSD1306 display2(128, 64, &Wire1, -1, 400000UL);

const char* ssid = "UPC0520860";
const char* password = "Ae8kdjbmaerx";
int current_screen = 0, currentStateCLK, lastStateCLK;
long lastButtonPress;

//wifi password setup
int letterIndex = 0;
char list_of_nonC_letters [] [2] = {{"a"}, {"b"}, {"c"}, {"d"}, {"e"}, {"f"}, {"g"}, {"h"}, {"i"}, {"j"}, {"k"}, {"l"}, {"m"}, {"n"}, {"o"}, {"p"}, {"q"}, {"r"}, {"s"}, {"t"}, {"u"}, {"v"}, {"w"}, {"x"}, {"y"}, {"z"}, {"0"}, {"1"}, 
{"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"!"}, {"@"}, {"#"}, {"$"}, {"%"}, {"^"}, {"&"}, {"*"}, {"("}, {")"}, {"."}, {","}, {":"}, {";"}, {"'"}, {'"'}, {"/"}, {"?"}, {"|"}, {"["}, {"]"}, {"{"}, {"}"}, {"<"}, {">"},};
char list_of_C_letters [] [2] = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}, {"F"}, {"G"}, {"H"}, {"I"}, {"J"}, {"K"}, {"L"}, {"M"}, {"N"}, {"O"}, {"P"}, {"Q"}, {"R"}, {"S"}, {"T"}, {"U"}, {"V"}, {"W"}, {"X"}, {"Y"}, {"Z"}, {"0"}, {"1"}, 
{"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"!"}, {"@"}, {"#"}, {"$"}, {"%"}, {"^"}, {"&"}, {"*"}, {"("}, {")"}, {"."}, {","}, {":"}, {";"}, {"'"}, {'"'}, {"/"}, {"?"}, {"|"}, {"["}, {"]"}, {"{"}, {"}"}, {"<"}, {">"},};
String passwordSet = "", letter = "";

void setup() {
  Serial.begin(9600);

  pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);


  Wire1.begin(41,42);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display2.clearDisplay();
  display2.display();
  display2.setTextSize(1);
  display2.setTextColor(WHITE); 
}

void loop() {
  //------Display init and frames------
  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);

  //------Insert letter to "text"------
  if(digitalRead(SW) == LOW){
    if(millis() - lastButtonPress > 50){
      passwordSet = passwordSet + letter;
      Serial.print(passwordSet);
      
    }
    lastButtonPress = millis();
  }

  //------Encoder based keyboard------
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
      letterIndex--;
      if(letterIndex < 0) letterIndex = 60;
    }
    else {
      letterIndex++;
      if(letterIndex > 60) letterIndex = 0;
    }
    display2.setCursor(5,5);
    display2.print("letter: ");
    
    //Capital letter or not
    display2.setCursor(47,5);
    if(digitalRead(BT2) == LOW){
      if(millis() - lastButtonPress > 50){
        display2.print(list_of_C_letters[letterIndex]);
        letter = list_of_C_letters[letterIndex];
      }
      lastButtonPress = millis();
    }
    else{
      display2.print(list_of_nonC_letters[letterIndex]);
      letter = list_of_nonC_letters[letterIndex];
    }
    //print written text on a screen
    display2.setCursor(5, 15);
    display2.print("PW: ");
    display2.setCursor(24,15);
    display2.print(passwordSet);
    display2.display(); 
  }
  lastStateCLK = currentStateCLK;
  delay(1);  
  
  //------return button------
  if(digitalRead(BT3) == LOW){
    if(millis() - lastButtonPress > 50) {
      //to add
	  } 
  lastButtonPress = millis();
  }
}
