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
- screen 7 -> Settings
- screen 8 -> DevTools
- screen 9 -> Time settings
- screen 10 -> Stopwatch
*/

//------"changable" variables------
int main_utc = 1;
bool sleeping = false; 
bool music_on = false;
bool active = true;
bool devTools = true;

//------libraries------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"
#include <virtuabotixRTC.h>
#include <Time.h>
#include "WiFi.h"
#include <dht11.h>

//------variables------
//menu
const int NUM_ITEMS = 7, MAX_ITEM_LENGTH = 20;
char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {
  { "Time/Temp" }, 
  { "Stats" }, 
  { "Timer" }, 
  { "Wifi scaner" }, 
  { "Volt/Ammeter" },
  { "Settings" },
  { "Stopwatch" },
 };
int bt_select = 0, item_selected = 0, item_sel_previous, item_sel_next;
//encoder and buttons
int current_screen = 0, currentStateCLK, lastStateCLK;
int first = 0;
unsigned long lastButtonPress = 0;
int setting = 0, settings = 0, settings1 = 0, settings_time, bezelPos = 0, lastEstate, btnStateE, devState1 = LOW, time_chosed_btn;
//stats
int happines = 100, hp = 100, doziness = 0, sun = 100;
//"delay"
unsigned long tc1, tc2, t1, t2, t3, t4, t5, ms;
//RTC
int hours_NFU;
String minutes_NFU, seconds_NFU;
//timer
int time_m = 0, time_s = 0, time_for = 0, time_for_2 = 0, button = 0, first_m = 0;
String timer_str = "";
//stopwatch
bool stoper_start = false, lap = false;
int seconds = 0, minutes = 0, seconds_lap = 0, minutes_lap = 0;
long t_stoper;
int stopState = LOW;
String lap_str = "", stop_str = "";
//dev tools
String onoff_sleep = "OFF";
//time settings
String time_zone = "";
//wifi
int scroll, minScroll, network, wifi_enc;
//temp, humid
String temp, humid;

//------define------
#define CLK 20
#define DT 19
#define SW 26
#define BT1 48
#define BT2 47
#define BT3 33
#define DHT11PIN 6

//------Displays init------
Adafruit_SSD1306 display1(128, 64, &Wire, 21, 500000UL);
Adafruit_SSD1306 display2(128, 64, &Wire1, -1, 400000UL);
//------RTC init------
virtuabotixRTC myRTC(40, 39, 34);
//------Multi core------
TaskHandle_t Task1;
TaskHandle_t Task2;
//------temp && humid init------
dht11 DHT11;

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

  //------millis------
  t1 = millis();
  t2 = millis();
  t3 = 1000;
  t4 = millis();
  t_stoper = millis();

  //------multicore------
  xTaskCreatePinnedToCore(core1, "core1", 150000, NULL, 1, &Task1, 0);
  delay(500); 
  xTaskCreatePinnedToCore(core2, "core2", 150000, NULL, 1, &Task1, 1);
  delay(500); 
}

//first core tasks
void core1( void * pvParameters ){
  for(;;){
    display2.setTextSize(1);
    display2.setTextColor(WHITE); 
    display2.setCursor(0, 0);
    tc1 = millis();
    ms = millis();
    myRTC.updateTime();
    
      
    //********************OTSI DIALOG WINDOW********************
    while(current_screen == 0){
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
	        if (millis() - lastButtonPress > 50) first++;
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

      //------To main menu------
      if(digitalRead(SW) == LOW){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 1;
		      }
		    lastButtonPress = millis();
      }
    }//end of while for screen 0

    //********************SCROLL MENU********************
    while(current_screen == 1){
      currentStateCLK = digitalRead(CLK);
      //------Encoder------
	    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		    if (digitalRead(DT) != currentStateCLK) {
          item_selected--;
          if (item_selected < 0) item_selected = NUM_ITEMS-1;
        }
        else {
          item_selected++;
          if (item_selected >= NUM_ITEMS) item_selected = 0;
        }
        //------displaying 3 items/names------
        item_sel_previous = item_selected - 1;
        if(item_sel_previous < 0)
          item_sel_previous = NUM_ITEMS - 1;
        item_sel_next = item_selected + 1;  
        if(item_sel_next >= NUM_ITEMS) 
          item_sel_next = 0;
        //main option
        display2.clearDisplay();  
        display2.setCursor(25, 29);
        display2.println(menu_items[item_selected]);   
        display2.drawBitmap(4, 24, (const unsigned char *)icons[item_selected], 16, 16, WHITE); 
        //previous option
        display2.setCursor(25, 7);
        display2.println(menu_items[item_sel_previous]); 
        display2.drawBitmap(4, 2, (const unsigned char *)icons[item_sel_previous], 16, 16, WHITE);    
        //next option
        display2.setCursor(25,51);    
        display2.println(menu_items[item_sel_next]);   
        display2.drawBitmap(4, 46, (const unsigned char *)icons[item_sel_next], 16, 16, WHITE);  
        //other
        display2.drawBitmap(120, 0, (const unsigned char *)dots, 8, 64, WHITE);
        display2.drawRect(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS, WHITE);   
        display2.drawBitmap(0, 22, (const unsigned char *)bezel, 124, 20, WHITE);        
        display2.display();
        }
      lastStateCLK = currentStateCLK;
      delay(1);
      //------Main menu navigation------
      //to Volt/Ammeter
      if(digitalRead(SW) == LOW && item_selected == 4){
        if (millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 2;
		      }
		    lastButtonPress = millis();
      }
      //to Wifi scanner
      else if(digitalRead(SW) == LOW && item_selected == 3){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 3;
		      }
		    lastButtonPress = millis();
      }
      //to Timer
      else if(digitalRead(SW) == LOW && item_selected == 2){
        if (millis() - lastButtonPress > 50) {
          fix_3();
          current_screen = 4;
		      }
		    lastButtonPress = millis();
      }
      //to Stats
      else if(digitalRead(SW) == LOW && item_selected == 1){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 5;
		      }
		    lastButtonPress = millis();
      }
      //to Time&Temp
      else if(digitalRead(SW) == LOW && item_selected == 0){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 6;
		      }
		    lastButtonPress = millis();
      }
      //to Settings
      else if(digitalRead(SW) == LOW && item_selected == 5){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 7;
		      }
		    lastButtonPress = millis();
      }
      //to Stopwatch
      else if(digitalRead(SW) == LOW && item_selected == 6){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 10;
		      }
		    lastButtonPress = millis();
      }
      //------Return button------
      if(digitalRead(BT3) == LOW){
        if (millis() - lastButtonPress > 50) {
          current_screen = 0;
	  	  } 
  		  lastButtonPress = millis();
      }

    }//end of while for screen 1

    //********************VOLT/AMMETER WINDOW********************
    while(current_screen == 2) {
      display2.clearDisplay();
      display2.display();

      //------Return button------
      if(digitalRead(BT3) == LOW){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 1;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 2

    //********************WIFI SCANER********************
    while(current_screen == 3) {
      display2.clearDisplay();
      display2.display();

      //------Return button------
      if(digitalRead(BT3) == LOW){
        if(millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 1;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 3

    //********************STOPWATCH********************
    while(current_screen == 10){
      tc1 = millis();
      //------encoder start, lap start------
      if(digitalRead(SW) == LOW){
        if (millis() - lastButtonPress > 50) {
          if(stopState){
            stoper_start = false;
            seconds = 0;
            minutes = 0;
          }
          else{
            stoper_start = true;
            seconds = 0;
            minutes = 0;
          }
          stopState = !stopState;
        }
		    lastButtonPress = millis();
      }
      if(digitalRead(BT2) == LOW) lap = true;
      //------stopwatch loop------
      if(stoper_start){
        if(tc1-t_stoper > 1000){
          seconds++;
          if(seconds == 60){
            minutes++;
            seconds = 0;
          }
          if(minutes > 99){
            minutes = 0;
            stoper_start = false;
          }
          //lap
          if(lap){
            seconds_lap = seconds;
            minutes_lap = minutes;
            seconds = 0;
            minutes = 0;
            lap = false;
          }
        t_stoper = tc1;
        }
      }
      //------display------
      //stopwatch
      display2.clearDisplay();
      display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
      //if for converting to e.g. 00:00
      if(minutes < 10) stop_str = "0" + String(minutes) + ":" + String(seconds);
      if(seconds < 10) stop_str = String(minutes) + ":" + "0" + String(seconds);
      if(minutes < 10 && seconds < 10) stop_str = "0" + String(minutes) + ":" + "0" + String(seconds);
      //display time
      display2.setCursor(10, 22);
      display2.setTextSize(2); 
      display2.println(stop_str);
      //lap
      display2.setTextSize(1);
      display2.setCursor(85, 20);
      display2.print("Lap:");
      //if for converting to e.g. 00:00
      if(minutes_lap < 10) lap_str = "0" + String(minutes_lap) + ":" + String(seconds_lap-1);
      if(seconds_lap < 10) lap_str = String(minutes_lap) + ":" + "0" + String(seconds_lap-1);
      if(seconds_lap == 0) lap_str = String(minutes_lap) + ":" + "0" + String(seconds_lap);
      if(minutes_lap < 10 && seconds_lap < 10) lap_str = "0" + String(minutes_lap) + ":" + "0" + String(seconds_lap-1);
      if(minutes_lap < 10 && seconds_lap == 0) lap_str = "0" + String(minutes_lap) + ":" + "0" + String(seconds_lap);
      //display time lap
      display2.setCursor(85, 30);
      display2.print(lap_str);
      display2.display();
      //------Return button------
      if(digitalRead(BT3) == LOW){
        if(millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 1;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 10

    //********************TIMER********************
    while(current_screen == 4){
      currentStateCLK = digitalRead(CLK);
      //------start button------
      if(digitalRead(SW) == LOW && current_screen == 4){
        if (millis() - lastButtonPress > 50) {
          time_for = setting * 60;
          button = time_for;
	  	  }
  		  lastButtonPress = millis();
      }
      //------encoder------
	    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		    if (digitalRead(DT) != currentStateCLK) {
          setting--;
          if(setting < 0){
            setting = 0;
          }
        }
        else {
          setting++;
          if(setting > 99){
            setting = 99;
          }
        }
        display2.clearDisplay();
        display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
        //display timer
        if(setting < 10) timer_str = "0" + String(setting) + ":" + "00";
        else timer_str = String(setting) + ":" + "00";
        display2.setCursor(10, 22);
        display2.setTextSize(2); 
        display2.print(timer_str);
        //text - time left
        display2.setTextSize(1);
        display2.setCursor(85, 22);
        display2.print("< Time");
        display2.setCursor(85, 32);
        display2.print("  left");
        display2.display();
      }
      lastStateCLK = currentStateCLK;
      delay(1);
      //------timer loop------
      while(button >= 1){
        for (time_for; time_for>=0; time_for--) {
          time_m=time_for/60;
          time_for_2=time_m;
          button--;
        for (time_for_2; time_for_2>=1; time_for_2--){
          if(time_for>60){
          time_s=time_for-(60*time_m);
          }
        }
        if(time_for<=60){
          time_s=time_for;
        }
        display2.clearDisplay();
        display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
        //display timer
        if(time_m < 10) timer_str = "0" + String(time_m) + ":" + String(time_s);
        if(time_s < 10) timer_str = String(time_m) + ":" + "0" + String(time_s);
        if(time_m < 10 && time_s < 10) timer_str = "0" + String(time_m) + ":" + "0" + String(time_s);
        display2.setCursor(10, 22);
        display2.setTextSize(2); 
        display2.print(timer_str);
        //text - time left
        display2.setTextSize(1);
        display2.setCursor(85, 22);
        display2.print("< Time");
        display2.setCursor(82, 32);
        display2.print("  left");
        display2.display(); 
        delay(1000);
        }
      }
      //------Return button------
      if(digitalRead(BT3) == LOW){
        if(millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 1;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 4

    //********************STATS********************
    while(current_screen == 5) {
      display2.clearDisplay();
      display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE); 
      //------Stats------
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
      //------Return button------
      if(digitalRead(BT3) == LOW){
        if(millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 1;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 5


    //********************TIME/TEMP********************
    while(current_screen == 6) {
      myRTC.updateTime();
      //------date and time convertion------
      String date = String(myRTC.dayofmonth) + "/" + String(myRTC.month) + "/" + String(myRTC.year);
      String time = String(hours_NFU) + ":" +  String(minutes_NFU) + ":" + String(seconds_NFU);
      //------time and humid read------
      int chk = DHT11.read(DHT11PIN);
      temp = String((float)DHT11.temperature);
      humid = String((float)DHT11.humidity);
      //------display------
      display2.clearDisplay();
      display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
      //date
      display2.setCursor(5,5);
      display2.print("Date: ");
      display2.setCursor(40,5);
      display2.print(date);
      //time
      display2.setCursor(5,15);
      display2.print("Time: ");
      display2.setCursor(40,15);
      display2.print(time);
      //temp
      display2.setCursor(5,25);
      display2.print("Temp('C): ");
      display2.setCursor(80,25);
      display2.print(temp);
      //humid
      display2.setCursor(5,35);
      display2.print("Humidity(%): ");
      display2.setCursor(80,35);
      display2.print(humid);
      display2.display();
      //------Return button------
      if(digitalRead(BT3) == LOW){
        if(millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 1;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 6

    //********************SETTINGS********************
    while(current_screen == 7) {
      currentStateCLK = digitalRead(CLK);
      display2.clearDisplay();
      //------Encoder------
      if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		    if (digitalRead(DT) != currentStateCLK) {
          settings--;
          if(settings == 0)
            settings = 3;
        }
        else {
          settings++;
          if(settings > 3)
            settings = 1;
        }
        //bezel position
        switch(settings){
          case 1:
            bezelPos = 5;
            break;
          case 2:
            bezelPos = 15;
            break;
          case 3:
            bezelPos = 25;
            break;
        }
        display2.drawRect(5, bezelPos, 2, 7, WHITE);
        display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
        display2.setCursor(10, 5);
        display2.println("Time zone");
        display2.setCursor(10, 15);
        display2.println("Place holder 2");
        //dev tools on/off
        if(devTools){
          display2.setCursor(10, 25);
          display2.println("Dev Tools");   
        }
        display2.display();
      }
      lastStateCLK = currentStateCLK;
      delay(1);
      //------Settings navigation------
      //to dev tools
      if(digitalRead(SW) == LOW &&settings == 3 && devTools){
        if (millis() - lastButtonPress > 50){
          fix_2();
          current_screen=8;
	  	    }
  		  lastButtonPress = millis();
      }
      //to time settings
      else if(digitalRead(SW) == LOW && settings == 1){
        if (millis() - lastButtonPress > 50){
          fix_2();
          current_screen=9;
	  	    }
  		  lastButtonPress = millis();
      }
      //------Return button------
      if(digitalRead(BT3) == LOW){
        if (millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 1;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 7

    //********************TIME SETTINGS********************
    while(current_screen == 9){
      //------Return button------
      //to settings
      if(digitalRead(BT3) == LOW){
        if(millis() - lastButtonPress > 50) {
          fix_4();
          current_screen = 7;
	  	    } 
  		  lastButtonPress = millis();
      }
    }//end of while for screen 9

    //********************DEV TOOLS********************
    while(current_screen == 8) {
      currentStateCLK = digitalRead(CLK);
      display2.clearDisplay();
      //------sleep toggle button------
      if(digitalRead(SW) == LOW && settings1 == 1){
        if (millis() - lastButtonPress > 50) {
          fix_4();
          if(devState1){
            onoff_sleep = "OFF";
            sleeping = false; 
            active = false;
          }
          else{
            onoff_sleep = "ON";
            sleeping = true; 
            active = false;
          }
          devState1 = !devState1;
	  	  }
  	  lastButtonPress = millis();
      }
      //------encoder------
      if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		    if (digitalRead(DT) != currentStateCLK) {
          settings1--;
          if(settings1 == 0)
            settings1 = 5;
        }
        else {
          settings1++;
          if(settings1 > 5)
            settings1 = 1;
        }
        //bezel position
        switch(settings1){
          case 1:
            bezelPos = 4;
            break;
          case 2:
            bezelPos = 14;
            break;
          case 3:
            bezelPos = 24;
            break;
          case 4:
            bezelPos = 34;
            break;
          case 5:
            bezelPos = 44;
            break;
        }
        display2.drawRect(5, bezelPos, 2, 7, WHITE);
        display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
        //sleep
        display2.setCursor(10, 4);
        display2.println("Sleep: ");
        display2.setCursor(50, 4);
        display2.println(onoff_sleep);
        display2.display();
      }
      lastStateCLK = currentStateCLK;
      delay(1);
      //------Return button------
      //to settings
      if(digitalRead(BT3) == LOW){
        if (millis() - lastButtonPress > 50) {
          fix_4();
          current_screen = 7;
	  	    } 
  		  lastButtonPress = millis();
      } 
    }//end of while for screen 8

  }//end of for
}

//second core tasks
void core2( void * pvParameters ){
  for(;;){
    tc2 = millis();
    display1.clearDisplay();

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

    //********************OTSI GRAPHICS******************
    if(hours_NFU >= 6 && active || hours_NFU < 22 && active)
      sleeping = false;
    if(hours_NFU >= 22 && active || hours_NFU < 6 && active)
      sleeping = true;
    //------Otsi normal------
    if(!sleeping && !music_on){
      if(tc2-t1 > 125){
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
    //------Otsi sleeping
    if(sleeping && !music_on){
      if(tc2-t2 > 1000){
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
    //------Otsi music------
    if(music_on){
      if(tc2-t4 > 500){
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_music1, 128, 64, WHITE);
        display1.display();
      }
      if(tc2-t4 > 850){
        display1.drawBitmap(0, 0, (const unsigned char *)otsi_music2, 128, 64, WHITE);
        display1.display();
        t4 = tc2;
      }
    }
  }//end of for
}

void loop(){
}

void fix_1(){
  currentStateCLK = 1;
  lastStateCLK = 0;
}
void fix_2(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  item_selected--;
}
void fix_3(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  setting--;
  item_selected--;
}
void fix_4(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  settings1--;
}