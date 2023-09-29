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

//------libraries------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"

//------variables------
const int NUM_ITEMS = 5; 
const int MAX_ITEM_LENGTH = 20;
char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {
  { "Time & Temp" }, 
  { "Stats" }, 
  { "Timer" }, 
  { "Wifi scaner" }, 
  { "Volt/Ammeter" }, 
 };
int bt_select = 0;
int item_selected = 0;
int item_sel_previous;
int item_sel_next;
int current_screen = 0;   // 0 = menu, 1 = screenshot, 2 = qr
int currentStateCLK;
int lastStateCLK;
int first = 0;
unsigned long lastButtonPress = 0;



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
  //secibd display
  Wire1.begin(41,42);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display2.clearDisplay();
  display2.display();
}


void loop() {
  display2.setTextSize(1);
  display2.setTextColor(WHITE); 
  display2.setCursor(0, 0);

  //------button encoder------
	int btnStateE = digitalRead(SW);

  //********************OTSI DIALOG WINDOW********************
  if (current_screen == 0) {
    
    if(first == 0){
      //Otsi graphic
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_normal, 128, 64, WHITE);
      display1.display();

      //itroduction
      display2.clearDisplay();
      display2.setCursor(0,0);    
      display2.println("HI I'm OTSI");
      display2.setCursor(0,9);
      display2.println("Your personal");
      display2.setCursor(0,18);
      display2.println("cyberpet and");
      display2.setCursor(0,27);
      display2.println("electronic helper");
      display2.display();
      if (digitalRead(BT1) == LOW){
	      if (millis() - lastButtonPress > 50) {
			  first++;
		    }
		  lastButtonPress = millis();
	    }  
      
    }
    else{
      display2.clearDisplay();
      display2.println("TEST");
      display2.display(); 
    }
    
    if (btnStateE == LOW){
	    if (millis() - lastButtonPress > 50) {
			  current_screen=1;
		  }
		  lastButtonPress = millis();
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
      if (item_sel_previous < 0)
        item_sel_previous = NUM_ITEMS - 1;
      item_sel_next = item_selected + 1;  
      if (item_sel_next >= NUM_ITEMS) 
        item_sel_next = 0;

      //------highlight------
      display2.clearDisplay();
      display2.drawBitmap(0, 22, (const unsigned char *)bezel, 124, 20, WHITE);   

      //------first option------
      display2.setCursor(25, 7);
      display2.println(menu_items[item_sel_previous]); 
      display2.drawBitmap(4, 2, (const unsigned char *)icons[item_sel_previous], 16, 16, WHITE);          

      //------second option------  
      display2.setCursor(25, 29);
      display2.println(menu_items[item_selected]);   
      display2.drawBitmap(4, 24, (const unsigned char *)icons[item_selected], 16, 16, WHITE); 

      //------third option------
      display2.setCursor(25,51);    
      display2.println(menu_items[item_sel_next]);   
      display2.drawBitmap(4, 46, (const unsigned char *)icons[item_sel_next], 16, 16, WHITE);  

      //------scrollbar------
      //scrollbar background
      display2.drawBitmap(120, 0, (const unsigned char *)dots, 8, 64, WHITE);
      //scrollbar handle
      display2.drawRect(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS, WHITE);         
      display2.display();
    }

    //------set screen of a selected item------
    //Volt/Ammeter
    if((digitalRead(BT2)== LOW)&&(item_selected==4)){
      current_screen = 2;
      item_selected = 0;
    }
    //WiFi scaner
    else if((digitalRead(BT2) == LOW)&&(item_selected==3)){
      current_screen = 3;
      item_selected = 0;
    }
    //Timer
    else if((digitalRead(BT2) == LOW)&&(item_selected==2)){
      current_screen = 4;
      item_selected = 0;
    }
    //Stats
    else if((digitalRead(BT2) == LOW)&&(item_selected==1)){
      current_screen = 5;
      item_selected = 1;
    }
    //Temp+Time
    else if((digitalRead(BT2) == LOW)&&(item_selected==0)){
      current_screen = 6;
      item_selected = 2;
    }

    //------return button------
    if(digitalRead(BT3) == LOW){
      if (millis() - lastButtonPress > 50) {
        current_screen = 0;
        item_selected = 0;
		    }
		  lastButtonPress = millis();
    }
  }

  //********************VOLT/AMMETER WINDOW********************
  if (current_screen == 2) {
    display2.clearDisplay();
    display2.display();
    
    //CODE

    if(digitalRead(BT3) == LOW){
      if (millis() - lastButtonPress > 50) {
        current_screen=1;
		    }
		  lastButtonPress = millis();
    }
  }

  //********************WIFI SCANER********************
  if (current_screen == 3) {
    display2.clearDisplay();
    display2.display();
    
    //CODE

    if(digitalRead(BT3) == LOW){
      if (millis() - lastButtonPress > 50) {
        current_screen=1;
		    }
		  lastButtonPress = millis();
    }
  }

  //********************TIMER********************
  if (current_screen == 4) {
    display2.clearDisplay();
    display2.display();
    
    //CODE

    if(digitalRead(BT3) == LOW){
      if (millis() - lastButtonPress > 50) {
        current_screen=1;
		    }
		  lastButtonPress = millis();
    }
  }

  //********************STATS********************
  if (current_screen == 5) {
    display2.clearDisplay();
    display2.display();
    
    //CODE

    if(digitalRead(BT3) == LOW){
      if (millis() - lastButtonPress > 50) {
        current_screen=1;
		    }
		  lastButtonPress = millis();
    }
  }

  //********************TIME & TEMP********************
  if (current_screen == 6) {
    display2.clearDisplay();
    display2.display();
    
    //CODE

    if(digitalRead(BT3) == LOW){
      if (millis() - lastButtonPress > 50) {
        current_screen=1;
		    }
		  lastButtonPress = millis();
    }
  }

  lastStateCLK = currentStateCLK;
  delay(1);
}  