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
const char* ssid = "UPC0520860";
const char* password = "Ae8kdjbmaerx";

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

//------variables------
//menu
const int NUM_ITEMS = 7, MAX_ITEM_LENGTH = 20;
char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {
  { "Time/Temp" }, 
  { "Volt/Ammeter" },
  { "Timer" }, 
  { "Stopwatch" },
  { "Wifi scaner" }, 
  { "Stats" }, 
  { "Settings" },
 };
int bt_select = 0, item_selected = 0, item_sel_previous, item_sel_next;
//faces
bool sleeping = false, music_on = false, active = false, love = false;
unsigned long t_love;
//encoder and buttons
int current_screen = 0, currentStateCLK, lastStateCLK;
int first = 0;
unsigned long lastButtonPress = 0;
int setting = 0, settings = 0, settings1 = 0, settings_time, bezelPos = 0, lastEstate, btnStateE, time_chosed_btn;
//stats
int happines = 15, hp = 100, doziness = 0, sun = 100; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool sun_change_bool = false;
long t_happiness, t_happinessMusicAdd;
//"delay"
unsigned long tc1, tc2, t1, t2, t3, t4, t5, ms, t_sun_1, t_sun_2;
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
String onoff_yawn = "OFF";
String onoff_music = "OFF";
int devState1 = LOW, devState2 = LOW, devState3 = LOW, dev_activate=0;
bool DEVyawn_on = false, devTools = false;
long t_activate_dev;
//time settings
String time_zone = "";
//wifi
int scroll, minScroll, network, wifi_enc;
//temp, humid
String temp, humid;
//otsi music
int timeMusic, listeningTime, randomMusic = 0;
long t_otsi_music, t_music_titles, tc_music, music_dev = false;
bool music_ask = false, start_music_animAfunction = false, song = false;
char list_of_songs [20] [19] = { {"Photographs"}, {"Loafers"}, {"Luminary"}, {"Brainwashed"}, {"Bad Things"}, {"Marmur"}, {"Trauma"}, {"Nosedive"}, {"Jailbreak"}, {"Dream"}, 
{"The Ride"}, {"Nightfall"}, {"Dusk"}, {"New High"}, {"After Dark"}, {"Sunset Lover"}, {"Neo-Tokyo"}, {"Infinite"}, {"Cool Kids"}, {"Sweden"},};
char list_of_singers [20] [19] = { {"Ed Sheeran"}, {"BoyWithUke"}, {"Joel Sunny"}, {"Tom MacDonald"}, {"BoyWithUke"}, {"Taco Hemingway"}, {"BoyWithUke"}, {"BoyWithUke"}, {"Astraleazar"}, {"Kalax"}, 
{"Kalax"}, {"Timecop1983"}, {"Lucy In Disguise"}, {"Trevor Something"}, {"Mr.Kitty"}, {"Petit Biscuit"}, {"Scandroid"}, {"Valence"}, {"Echosmith"}, {"C418"},};
//time update based on wifi
const char* ntpServer = "pool.ntp.org";
long  gmtOffset_sec = 0, t_WT;
int  daylightOffset_sec = 0;
String ISconnected = "";
String WiFitime = "";
int Ahour, Amin, Asec, Aday, Amonth, Ayear, Awday;


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

  //RTC update
  WiFiupdateTime();

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
  t_activate_dev = millis();
  t_happiness = millis();
  t_love = millis();

  //------multicore------
  xTaskCreatePinnedToCore(core1, "core1", 150000, NULL, 1, &Task1, 0);
  delay(500); 
  //xTaskCreatePinnedToCore(core2, "core2", 150000, NULL, 1, &Task1, 1);
  //delay(500); 

}

//first core tasks
void core1( void * pvParameters ){
  for(;;){
    display2.setTextSize(1);
    display2.setTextColor(WHITE); 
    display2.setCursor(0, 0);
    tc1 = millis();
    ms = millis();
      
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
        display2.clearDisplay();
        display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE); 
        display2.drawBitmap(5, 55, (const unsigned char *)pet, 24, 8, WHITE);
        display2.drawBitmap(30, 54, (const unsigned char *)interact, 40, 8, WHITE);

        //------pet------
        if(digitalRead(BT3) == LOW){
          if(millis() - lastButtonPress > 50){
          love = true;
            if(tc1-t_love > 6000){
              if(happines < 92) happines = happines + 8;
              else if(happines >= 92) happines = 100;
              t_love = tc1;
            }
          }
          lastButtonPress = millis();
        }

        //------when listening to music------
        if(digitalRead(BT2) == LOW && start_music_animAfunction){
          if(millis() - lastButtonPress > 50){
            song = true;
            t_music_titles = millis();
            if(happines < 95) happines = happines + 5;
            else if(happines >= 95) happines = 100;
          }
          lastButtonPress = millis();
        }
        if(song){
          tc_music = millis();
          //what are you listening to?
          if(tc_music-t_music_titles > 0 && tc_music-t_music_titles < 3000){
            display2.setCursor(5, 5);
            display2.print("ME: Otsi!");
            display2.setCursor(5, 15);
            display2.print("What are you");
            display2.setCursor(5, 25);
            display2.print("listening to?");
          }
          //i'm listening to...
          else if(tc_music-t_music_titles > 3000 && tc_music-t_music_titles < 6000){
            display2.setCursor(5, 5);
            display2.print("OTSI: Right now I'm");
            display2.setCursor(5, 15);
            display2.print("listening to...");
          }
          //music from two lists
          else if(tc_music-t_music_titles > 6000 && tc_music-t_music_titles < 10000){
            display2.setCursor((120-(String(list_of_songs[randomMusic]).length()*5))/2, 12);
            display2.print(list_of_songs[randomMusic]);
            display2.setCursor(59, 22);
            display2.print("by");
            display2.setCursor((120-(String(list_of_singers[randomMusic]).length()*5))/2, 32);
            display2.print(list_of_singers[randomMusic]);
          }
          else if(tc_music-t_music_titles > 10000){
            display2.clearDisplay();
            song = false;
            t_music_titles = tc_music;
          }
        }//end of if when listening to music
        
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
      tc1 = millis();
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
      //------Activate dev tools------
      if(digitalRead(BT1) == LOW){
        if (millis() - lastButtonPress > 50) dev_activate++;
        lastButtonPress = millis();
      }

      if(tc1-t_activate_dev > 3000){
        dev_activate = 0;
        t_activate_dev = millis();
      }
      if(dev_activate >= 10) devTools = true;
      //------Main menu navigation------
      //to Volt/Ammeter
      if(digitalRead(SW) == LOW && item_selected == 1){
        if (millis() - lastButtonPress > 50) {
          fix_1();
          current_screen = 2;
		      }
		    lastButtonPress = millis();
      }
      //to Wifi scanner
      else if(digitalRead(SW) == LOW && item_selected == 4){
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
      else if(digitalRead(SW) == LOW && item_selected == 5){
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
      else if(digitalRead(SW) == LOW && item_selected == 6){
        if (millis() - lastButtonPress > 50) {
          fix_2();
          current_screen = 7;
		      }
		    lastButtonPress = millis();
      }
      //to Stopwatch
      else if(digitalRead(SW) == LOW && item_selected == 3){
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
      display2.setCursor(105,5);
      display2.print(happines);
      //hp
      display2.setCursor(5,14);
      display2.print("hp:");
      display2.setCursor(105,14);
      display2.print(hp);
      //doziness
      display2.setCursor(4,23);
      display2.print("doziness:");
      display2.setCursor(105,23);
      display2.print(doziness);
      //sunlight
      display2.setCursor(5,32);
      display2.print("sunlight:");
      display2.setCursor(105,32);
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
      display2.setCursor(62,25);
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
        if(devTools){
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
        }
        else if(!devTools){
          switch(settings){
            case 1:
              bezelPos = 5;
              break;
            case 2:
              bezelPos = 15;
              break;
          }
        }
        display2.drawRect(5, bezelPos, 2, 7, WHITE);
        display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
        display2.setCursor(10, 5);
        display2.println("Time zone");
        display2.setCursor(10, 15);
        display2.println("Update Time");
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
      tc1 = millis();
      currentStateCLK = digitalRead(CLK);
      display2.clearDisplay();
      //------sleep toggle button------
      if(digitalRead(SW) == LOW && settings1 == 1){
        if (millis() - lastButtonPress > 50) {
          fix_4();
          if(devState1){
            onoff_sleep = "OFF";
            active = false;
          }
          else{
            onoff_sleep = "ON"; 
            active = true;
          }
          devState1 = !devState1;
	  	  }
  	  lastButtonPress = millis();
      }
      //------yawn toggle button------
      if(digitalRead(SW) == LOW && settings1 == 2){
        if (millis() - lastButtonPress > 50) {
          fix_4();
          if(devState2){
            onoff_yawn = "OFF";
            DEVyawn_on = false;
          }
          else{
            onoff_yawn = "ON";
            DEVyawn_on = true;
          }
          devState2 = !devState2;
	  	  }
  	  lastButtonPress = millis();
      }
      //------music toggle button------
      if(digitalRead(SW) == LOW && settings1 == 3){
        if (millis() - lastButtonPress > 50) {
          fix_4();
          if(devState3){
            onoff_music = "OFF";
            music_dev = false;
            start_music_animAfunction = false;
          }
          else{
            onoff_music = "ON";
            music_dev = true;
            start_music_animAfunction = true;
          }
          devState3 = !devState3;
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
        //yawn
        display2.setCursor(10, 14);
        display2.println("Yawn: ");
        display2.setCursor(50, 14);
        display2.println(onoff_yawn);
        //music
        display2.setCursor(10, 24);
        display2.println("Music: ");
        display2.setCursor(50, 24);
        display2.println(onoff_music);
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
/*void core2( void * pvParameters ){
  for(;;){
  }//end of for
}
*/

void loop(){
  myRTC.updateTime();
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

  //********************OTSI LISTENING TO MUSIC******************
  listeningTime = random(5,15);
  //------if time------
  if(myRTC.hours+main_utc == 10 && myRTC.minutes == 00 || myRTC.hours+main_utc == 13 && myRTC.minutes == 00 || myRTC.hours+main_utc == 18 && myRTC.minutes == 00 || myRTC.hours+main_utc == 18 && myRTC.minutes == 00){ //if time for otsi to listen to music
    t_otsi_music = millis();
    start_music_animAfunction = true;
  }
  //------start the animation for listeningtime*30s time------
  if(tc2-t_otsi_music < listeningTime*30000 && start_music_animAfunction || music_dev){
    music_on = true;
    if(tc2-t4 > 500){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_music1, 128, 64, WHITE);
      display1.display();
    }
    if(tc2-t4 > 1000){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_music2, 128, 64, WHITE);
      display1.display();
      t4 = tc2;
    }
    music_ask = true;
  }
  //------do when time have passed------
  else if(tc2-t_otsi_music >= 30000 && start_music_animAfunction || !music_dev){
    start_music_animAfunction = false;
    music_on = false;
  }

  //********************STATS-doziness******************
  switch(myRTC.hours + main_utc){
    //add doziness
    case 12:
      doziness = 10;
      break;
    case 13:
      doziness = 20;
      break;
    case 14:
      doziness = 30;
      break;
    case 15:
      doziness = 40;
      break;
    case 16:
      doziness = 50;
      break;
    case 17:
      doziness = 60;
      break;
    case 18:
      doziness = 70;
      break;
    case 19:
      doziness = 80;
      break;
    case 20:
      doziness = 90;
      break;
    case 21:
      doziness = 100;
      break;
    //substract
    case 23:
      doziness = 85;
      break;
    case 24:
      doziness = 60;
      break;
    case 1:
      doziness = 45;
      break;
    case 2:
      doziness = 35;
      break;
    case 3:
      doziness = 20;
      break;
    case 4:
      doziness = 10;
      break;    
    case 5:
      doziness = 0;
      break;  
  }
  if(doziness == 70 || DEVyawn_on){
    if(myRTC.minutes == 30 && myRTC.seconds == 0 || DEVyawn_on){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }
  if(doziness == 80){
    if(myRTC.minutes == 20 && myRTC.seconds == 0 || myRTC.minutes == 40 && myRTC.seconds == 0){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }
  if(doziness == 90){
    if(myRTC.minutes == 15 && myRTC.seconds == 0 || myRTC.minutes == 30 && myRTC.seconds == 0 || myRTC.minutes == 45 && myRTC.seconds == 0 || myRTC.minutes == 59 && myRTC.seconds == 0){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }
  if(doziness == 100){
    if(myRTC.minutes == 10 && myRTC.seconds == 0 || myRTC.minutes == 20 && myRTC.seconds == 0 || myRTC.minutes == 30 && myRTC.seconds == 0 || myRTC.minutes == 40 && myRTC.seconds == 0 || myRTC.minutes == 50 && myRTC.seconds == 0 || myRTC.minutes == 59 && myRTC.seconds == 0){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }

  //********************STATS-sun******************
  if(sun < 20){
    if(tc2-t_sun_1 > 60000) sun_change_bool = true;
    if(sun_change_bool && tc2-t_sun_2 > 3000){
      hp = hp - 5;
      t_sun_2 = tc2;
    }
  }

  //********************STATS-happines******************
  if(tc2-t_happiness > 2000000){
    happines = happines - 5;
    t_happiness = tc2;
  }

  //********************OTSI GRAPHICS******************
  if(myRTC.hours+main_utc >= 6 || myRTC.hours+main_utc < 22)
    sleeping = false;
  if(myRTC.hours+main_utc >= 22 || myRTC.hours+main_utc < 6)
    sleeping = true;
  //------Otsi normal------
  if(!sleeping && !music_on && !active && !love){
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
  //------Otsi sleeping------
  if(sleeping && !music_on && !love || active){
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
  //------Otsi love------
  if(love && !sleeping){
    display1.clearDisplay();
    display1.drawBitmap(0, 0, (const unsigned char *)otsi_love, 128, 64, WHITE);
    display1.display();
    delay(500);
    love = false;
  }
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
void WiFiupdateTime(){
  t_WT = millis();
  //------connect to WiFi------
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  //------check the connection------
  while (WiFi.status() != WL_CONNECTED && millis()-t_WT < 5000) delay(500);
  if(WiFi.status() != WL_CONNECTED && millis()-t_WT > 5000) ISconnected = "NOT CONNECTED";
  else ISconnected = "CONNECTED";
  //------init and get the time------
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) WiFitime = "failed";
  //------disconect from WiFi------
  WiFi.disconnect();
  //------set RTC------
  if(ISconnected == "CONNECTED"){
  Ahour = timeinfo.tm_hour;
  Amin = timeinfo.tm_min;
  Asec = timeinfo.tm_sec;
  Aday = timeinfo.tm_mday;
  Amonth = timeinfo.tm_mon+1;
  Ayear = timeinfo.tm_year+1900;
  Awday = timeinfo.tm_wday;
  myRTC.setDS1302Time(Asec, Amin, Ahour, Awday, Aday, Amonth, Ayear);
  }
}