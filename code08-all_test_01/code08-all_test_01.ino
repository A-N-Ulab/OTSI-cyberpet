/*
Arduino cyberpet
Menu based on upiir's project: https://github.com/upiir/arduino_oled_menu/tree/main
second OLED's screens:
- screen 0 -> Main screen
- screen 1 -> Apps screen
- screen 2 -> Volt/Ammeter screen
- screen 3 -> Wifi scaner
- screen 4 -> Timer
- screen 5 -> Stats
- screen 6 -> Time & Temp
*/

//------"changable" variables------
int main_utc = 2;
bool sleeping = false; 
bool music_on = false;
bool active = true;

//------libraries------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"
#include <virtuabotixRTC.h>
#include <Time.h>

//------variables------
const int NUM_ITEMS = 5; 
const int MAX_ITEM_LENGTH = 20;
char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {
  { "Time/Temp" }, 
  { "Stats" }, 
  { "Timer" }, 
  { "Wifi scaner" }, 
  { "Volt/Ammeter" }, 
 };
int bt_select = 0;
int item_selected = 0;
int item_sel_previous;
int item_sel_next;
int current_screen = 0;
int currentStateCLK;
int lastStateCLK;
int first = 0;
unsigned long lastButtonPress = 0;
int happines = 100;
int hp = 100;
int doziness = 0;
int sun = 100;
unsigned long tc1, tc2, t1, t2, t3, t4;
int hours_NFU;
String minutes_NFU, seconds_NFU;

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

//------RTC init------
virtuabotixRTC myRTC(40, 39, 34);

//------Multi core------
TaskHandle_t Task1;
TaskHandle_t Task2;

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

  t1 = millis();
  t2 = millis();
  t4 = millis();

  xTaskCreatePinnedToCore(core1, "core1", 150000, NULL, 1, &Task1, 0);
  delay(500); 
  xTaskCreatePinnedToCore(core2, "core2", 150000, NULL, 1, &Task1, 1);
  delay(500); 

}

//first core tasks
void core1( void * pvParameters ) {
  for(;;){
    unsigned long ms = millis();
    tc1 = millis();
    display2.setTextSize(1);
    display2.setTextColor(WHITE); 
    display2.setCursor(0, 0);
    myRTC.updateTime();

    //********************FIXED HOURS, MINUTES, SECONDS******************
    //hours
    int h = myRTC.hours + main_utc;
    if(h > 24)
      hours_NFU = h - 24;
    else
      hours_NFU = h;
    //minutes
    int m = myRTC.minutes;
    if(m < 10){
      minutes_NFU = "";
      minutes_NFU += "0";
      minutes_NFU += String(m);
    }
    else
      minutes_NFU = String(m);
    //seconds
    int s = myRTC.seconds;
    if(s < 10){
      seconds_NFU = "";
      seconds_NFU += "0";
      seconds_NFU += String(s);
    }
    else
      seconds_NFU = String(s);
      
    //--------Encoder button for changing screens and back button--------
    int btnStateE = digitalRead(SW);

    //to select menu
    if(btnStateE == LOW && current_screen != 1){
      if (millis() - lastButtonPress > 50) {
        current_screen=1;
		    }
		  lastButtonPress = millis();
    }
    //to Volt/Ammeter
    else if(btnStateE == LOW && item_selected==4){
      if (millis() - lastButtonPress > 50) {
        current_screen=2;
	  	  }
  		lastButtonPress = millis();
    }
    //to WiFI scanner
    else if(btnStateE == LOW && item_selected==3){
      if (millis() - lastButtonPress > 50) {
        current_screen=3;
		    }
		  lastButtonPress = millis();
    }
    //to Timer
    else if(btnStateE == LOW && item_selected==2){
      if (millis() - lastButtonPress > 50) {
        current_screen=4;
		    }
		    lastButtonPress = millis();
    }
    //to Stats
    else if(btnStateE == LOW && item_selected==1){
      if (millis() - lastButtonPress > 50) {
        current_screen=5;
		    }
		  lastButtonPress = millis();
    }
    //to Time/Temp
    else if(btnStateE == LOW && item_selected==0){
      if (millis() - lastButtonPress > 50) {
        current_screen=6;
		    }
		  lastButtonPress = millis();
    }
    //return button
    if(digitalRead(BT3) == LOW && current_screen==1){
      if (millis() - lastButtonPress > 50) {
        current_screen = 0;
	  	  } 
  		lastButtonPress = millis();
    }

    //********************OTSI DIALOG WINDOW********************
    if (current_screen == 0){
      if(first == 0){
        //frame & buttons description
        display2.clearDisplay();
        display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
        display2.drawBitmap(5, 55, (const unsigned char *)pet, 24, 8, WHITE);
        display2.drawBitmap(30, 54, (const unsigned char *)interact, 40, 8, WHITE);  
        display2.drawBitmap(71, 55, (const unsigned char *)ok, 16, 8, WHITE);

        //introduction
        display2.setCursor(4,4);
        display2.print("Hi I'm OTSI");
        display2.setCursor(4,14);
        display2.print("Your cyberpet");
        display2.setCursor(4,24);
        display2.print("& electronic helper");
        display2.setCursor(4,34);
        display2.print("& much more...");
        display2.display();

        //buton to skip the introduction
        if (digitalRead(BT1) == LOW){
	        if (millis() - lastButtonPress > 50) {
			    first++;
		      }
		    lastButtonPress = millis();
	      }  
      }
      else{
        //frame & buttons description
        display2.clearDisplay();
        display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
        display2.drawBitmap(5, 55, (const unsigned char *)pet, 24, 8, WHITE);
        display2.drawBitmap(30, 54, (const unsigned char *)interact, 40, 8, WHITE);  
        display2.display();
      }
    }

    //********************SCROLL MENU********************
    if (current_screen == 1) {
      //------reading encoder's data------
      currentStateCLK = digitalRead(CLK);
	    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		    if (digitalRead(DT) != currentStateCLK) {
          item_selected = item_selected - 1;
          if (item_selected < 0) {
            item_selected = NUM_ITEMS-1;
          }
        }
        else {
          item_selected = item_selected + 1;
          if (item_selected >= NUM_ITEMS) {
            item_selected = 0;
          }
        }
      
        //------"infinite" item selection------
        item_sel_previous = item_selected - 1;
        if(item_sel_previous < 0)
          item_sel_previous = NUM_ITEMS - 1;
        item_sel_next = item_selected + 1;  
        if(item_sel_next >= NUM_ITEMS) 
          item_sel_next = 0;

        //Highlight
        display2.clearDisplay();
        display2.drawBitmap(0, 22, (const unsigned char *)bezel, 124, 20, WHITE);  

        //First option
        display2.setCursor(25, 7);
        display2.println(menu_items[item_sel_previous]); 
        display2.drawBitmap(4, 2, (const unsigned char *)icons[item_sel_previous], 16, 16, WHITE);    

        //Second option  
        display2.setCursor(25, 29);
        display2.println(menu_items[item_selected]);   
        display2.drawBitmap(4, 24, (const unsigned char *)icons[item_selected], 16, 16, WHITE); 

        //Third option
        display2.setCursor(25,51);    
        display2.println(menu_items[item_sel_next]);   
        display2.drawBitmap(4, 46, (const unsigned char *)icons[item_sel_next], 16, 16, WHITE);  

        //Scrollbar
        //-scrollbar background
        display2.drawBitmap(120, 0, (const unsigned char *)dots, 8, 64, WHITE);
        //-scrollbar handle
        display2.drawRect(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS, WHITE);         
        display2.display();
      }
    }

    //********************VOLT/AMMETER WINDOW********************
    if (current_screen == 2) {
      display2.clearDisplay();
      display2.display();

      //CODE
    }

    //********************WIFI SCANER********************
    if (current_screen == 3) {
      display2.clearDisplay();
      display2.display();
    
      //CODE
    }

    //********************TIMER********************
    if (current_screen == 4) {
      display2.clearDisplay();
      display2.display();
    
      //CODE
    }

    //********************STATS********************
    if (current_screen == 5) {
      display2.clearDisplay();
      display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
    
      //happiness
      display2.setCursor(5,5);
      display2.print("happiness:");
      display2.setCursor(107,5);
      display2.print(happines);

      //hp
      display2.setCursor(5,14);
      display2.print("hp:");
      display2.setCursor(107,14);
      display2.print(hp);

      //doziness
      display2.setCursor(4,23);
      display2.print("doziness:");
      display2.setCursor(107,23);
      display2.print(doziness);

      //sunlight
      display2.setCursor(5,32);
      display2.print("sunlight:");
      display2.setCursor(107,32);
      display2.print(sun);

      display2.display();
    }

    //********************TIME/TEMP********************
    if (current_screen == 6) {
      display2.clearDisplay();

      display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  

      String date = String(myRTC.dayofmonth) + "/" + String(myRTC.month) + "/" + String(myRTC.year);
      String time = String(hours_NFU) + ":" +  String(minutes_NFU) + ":" + String(seconds_NFU);

      display2.setCursor(5,5);
      display2.print("Date: ");
      display2.setCursor(40,5);
      display2.print(date);
      display2.setCursor(5,15);
      display2.print("Time: ");
      display2.setCursor(40,15);
      display2.print(time);
      display2.display();
    }

    lastStateCLK = currentStateCLK;
    delay(1);
  }
}

//second core tasks
void core2( void * pvParameters ){
  for(;;){
    tc2 = millis();

    //********************OTSI GRAPHICS******************
    Serial.println(hours_NFU);

    if(hours_NFU >= 6 && active || hours_NFU < 22 && active)
      sleeping = false;
    if(hours_NFU >= 22 && active || hours_NFU < 6 && active)
      sleeping = true;

    if(!sleeping && !music_on){
      if(tc2-t1 > 125){
        display1.clearDisplay();
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_normal, 128, 64, WHITE);
        display1.display();
      } 
      if(tc2-t1 > 3150){
        display1.clearDisplay();
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_blinking, 128, 64, WHITE);
        display1.display();
        t1 = tc2;
      } 
    }

    if(sleeping && !music_on){
      if(tc2-t2 > 1000){
        display1.clearDisplay();
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_sleeping1, 128, 64, WHITE);
        display1.display();
      }
      if(tc2-t2 > 2000){
        display1.clearDisplay();
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_sleeping2, 128, 64, WHITE);
        display1.display();
        t2 = tc2; 
      }
    }

    if(music_on){
      if(tc2-t4 > 500){
        display1.clearDisplay();
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_music1, 128, 64, WHITE);
        display1.display();
      }
      if(tc2-t4 > 850){
        display1.clearDisplay();
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_music2, 128, 64, WHITE);
        display1.display();
        t4 = tc2;
      }
    }
  }
}

void loop(){

}