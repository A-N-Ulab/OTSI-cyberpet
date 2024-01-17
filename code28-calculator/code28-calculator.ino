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
int letterIndex = 0, changePorS = 0, lenText;
char list_of_nonC_letters [] [2] = {{"a"}, {"b"}, {"c"}, {"d"}, {"e"}, {"f"}, {"g"}, {"h"}, {"i"}, {"j"}, {"k"}, {"l"}, {"m"}, {"n"}, {"o"}, {"p"}, {"q"}, {"r"}, {"s"}, {"t"}, {"u"}, {"v"}, {"w"}, {"x"}, {"y"}, {"z"}, {"0"}, {"1"}, 
{"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"!"}, {"@"}, {"#"}, {"$"}, {"%"}, {"^"}, {"&"}, {"*"}, {"("}, {")"}, {"."}, {","}, {":"}, {";"}, {"'"}, {'"'}, {"/"}, {"?"}, {"|"}, {"["}, {"]"}, {"{"}, {"}"}, {"<"}, {">"},};
char list_of_C_letters [] [2] = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}, {"F"}, {"G"}, {"H"}, {"I"}, {"J"}, {"K"}, {"L"}, {"M"}, {"N"}, {"O"}, {"P"}, {"Q"}, {"R"}, {"S"}, {"T"}, {"U"}, {"V"}, {"W"}, {"X"}, {"Y"}, {"Z"}, {"0"}, {"1"}, 
{"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"!"}, {"@"}, {"#"}, {"$"}, {"%"}, {"^"}, {"&"}, {"*"}, {"("}, {")"}, {"."}, {","}, {":"}, {";"}, {"'"}, {'"'}, {"/"}, {"?"}, {"|"}, {"["}, {"]"}, {"{"}, {"}"}, {"<"}, {">"},};
String passwordSet = "", SSIDset = "", letter = "", text = "";


//name variables
String otsisName = "", letterName = "";
bool nameChoosen = false;
int counterLetterName = 0;

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

void loop() {
  askForName();

}

void askForName(){
  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);

  //------Insert letter to "text"------
  if(digitalRead(SW) == LOW){
    if(millis() - lastButtonPress > 50){
      if(counterLetterName <= 4){
        otsisName = otsisName + letterName;
        counterLetterName++;
      }
      fix_5();
    }
    lastButtonPress = millis();
  }

  //------backspace------
  if(digitalRead(BT1) == LOW){
    if(millis() - lastButtonPress > 50){
      otsisName = otsisName.substring(0, otsisName.length()-1);
      if(counterLetterName >= 0) counterLetterName--;
      fix_5();
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
    display2.setCursor(5,25);
    display2.print("LETTER: ");
    display2.setCursor(50,25);
    //Capital letter or not
    if(digitalRead(BT2) == LOW){
      if(millis() - lastButtonPress > 50){
          display2.print(list_of_C_letters[letterIndex]);
          letterName = list_of_C_letters[letterIndex];
      }
    lastButtonPress = millis();
    }
    else{ 
      display2.print(list_of_nonC_letters[letterIndex]);
      letterName = list_of_nonC_letters[letterIndex];
    }
    //print written text on a screen
    display2.setCursor(5, 35);
    display2.print("NAME: ");
    display2.setCursor(35, 35);
    display2.print(otsisName);

    display2.setCursor(5, 5);
    display2.print("Set your otsi's name");
    display2.setCursor(6, 15);
    display2.print("max 5 characters");

    display2.display(); 
  } 
  lastStateCLK = currentStateCLK;
  delay(1);  
}

void fix_5(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  letterIndex--;
}
