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

//------"changable" variables------
int main_utc = 1, changeUTC = 1;
String ssid = "UPC0520860";
String password = "Ae8kdjbmaerx";

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
bool sleeping = false, music_on = false, active = false, love = false, fuckU = false;
unsigned long t_love, t_fuckU, t_fuckU1;
//encoder and buttons
int current_screen = 0, currentStateCLK, lastStateCLK, first = 0, setting = 0, settings = 0, settings1 = 0, settings_time, bezelPos = 0, lastEstate, btnStateE, time_chosed_btn;
unsigned long lastButtonPress = 0;
//stats
int happines = 100, hp = 100, doziness = 0, sun = 100, nextLvlGoal = 4, calcLvl = 0, lvl = 0, numOfInteractions = 0, amountOfwakes = 0;
bool sun_change_bool = false, lvlUp = false, updateStats = false, boolStats = false, hpHigh = true;
long t_happiness, t_happinessMusicAdd, t_happiness1, t_onTimeLast = 0, t_fromStart = 0, t_fromStart_firstRead, t_sun, t_sun1, t_save;
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
int seconds = 0, minutes = 0, seconds_lap = 0, minutes_lap = 0, stopState = LOW;
long t_stoper;
String lap_str = "", stop_str = "";
//dev tools
String onoff_sleep = "OFF", onoff_yawn = "OFF", onoff_music = "OFF";
int devState1 = LOW, devState2 = LOW, devState3 = LOW, dev_activate=0;
bool DEVyawn_on = false, devTools = false, inDEV = false;
long t_activate_dev;
//time settings
String time_zone = "", LastUpdated = "IDK";
int settingsT = 0, hourPM, dayPM, monPM, yearPM, mon_NFU, day_NFU, year_NFU, minutes_NFU_int, seconds_NFU_int, dst;
int years[] = {2024, 2028, 2032, 2036, 2040, 2044, 2048, 2052, 2056, 2060, 2064, 2068, 2072, 2076, 2080, 2084, 2088, 2092, 2096, 2100}, hRTC, mRTC, sRTC, dRTC, mmRTC, yRTC;
int times[] = {-11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; 
char list_of_timeZones[][19]{{"Samoa"}, {"Hawaii"}, {"Alaska"}, {"Pacific Time"}, {"Mountain Time"}, {"Central Time"}, {"New York"}, {"La Paz"}, {"Brasillia"}, 
{"Mid-Atlantic"}, {"Azores"}, {"London"}, {"Berlin"}, {"Helsinki"}, {"Moscow"}, {"Dubai"}, {"Islamabad"}, {"Dhaka"}, {"Bangkok"}, {"Singapore"}, {"Tokyo/Osaka"}, {"Sydney"}, {"Magadan"}, {"Fiji"}};
String plus, yORnTime = "N";
bool dstBool;
//wifi
int scroll, minScroll, network, wifi_enc;
//temp, humid
String temp, humid;
//otsi music
int timeMusic, listeningTime, randomMusic = 0, startMusic = 0;
long t_otsi_music, t_music_titles, tc_music, t_music_start;
bool music_ask = false, start_music_animAfunction = false, song = false, music_dev = false, IDKsomeBool = true;
char list_of_songs [20] [19] = { {"Photographs"}, {"Loafers"}, {"Luminary"}, {"Brainwashed"}, {"Bad Things"}, {"Marmur"}, {"Trauma"}, {"Nosedive"}, {"Jailbreak"}, {"Dream"}, 
{"The Ride"}, {"Nightfall"}, {"Dusk"}, {"New High"}, {"After Dark"}, {"Sunset Lover"}, {"Neo-Tokyo"}, {"Infinite"}, {"Cool Kids"}, {"Sweden"},};
char list_of_singers [20] [19] = { {"Ed Sheeran"}, {"BoyWithUke"}, {"Joel Sunny"}, {"Tom MacDonald"}, {"BoyWithUke"}, {"Taco Hemingway"}, {"BoyWithUke"}, {"BoyWithUke"}, {"Astraleazar"}, {"Kalax"}, 
{"Kalax"}, {"Timecop1983"}, {"Lucy In Disguise"}, {"Trevor Something"}, {"Mr.Kitty"}, {"Petit Biscuit"}, {"Scandroid"}, {"Valence"}, {"Echosmith"}, {"C418"},};
//time update based on wifi
const char* ntpServer = "pool.ntp.org";
long  gmtOffset_sec = 0, t_WT;
int  daylightOffset_sec = 0, Ahour, Amin, Asec, Aday, Amonth, Ayear, Awday;
bool ISconnected = false, Updated = false;
String WiFitime = "", TimeUpdated = "";
//WiFi scanner
String WiFi_scan_encryption = "", WiFi_scan_SSID = "", tooLong;
int WiFi_scan_len, WiFi_enc_len, max_len;
bool first_WiFi = true;
long t_s1, t_s2;
//wifi password setup
int letterIndex = 0, changePorS = 0, lenText;
char list_of_nonC_letters [] [2] = {{"a"}, {"b"}, {"c"}, {"d"}, {"e"}, {"f"}, {"g"}, {"h"}, {"i"}, {"j"}, {"k"}, {"l"}, {"m"}, {"n"}, {"o"}, {"p"}, {"q"}, {"r"}, {"s"}, {"t"}, {"u"}, {"v"}, {"w"}, {"x"}, {"y"}, {"z"}, {"0"}, {"1"}, 
{"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"!"}, {"@"}, {"#"}, {"$"}, {"%"}, {"^"}, {"&"}, {"*"}, {"("}, {")"}, {"."}, {","}, {":"}, {";"}, {"'"}, {'"'}, {"/"}, {"?"}, {"|"}, {"["}, {"]"}, {"{"}, {"}"}, {"<"}, {">"},};
char list_of_C_letters [] [2] = {{"A"}, {"B"}, {"C"}, {"D"}, {"E"}, {"F"}, {"G"}, {"H"}, {"I"}, {"J"}, {"K"}, {"L"}, {"M"}, {"N"}, {"O"}, {"P"}, {"Q"}, {"R"}, {"S"}, {"T"}, {"U"}, {"V"}, {"W"}, {"X"}, {"Y"}, {"Z"}, {"0"}, {"1"}, 
{"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"!"}, {"@"}, {"#"}, {"$"}, {"%"}, {"^"}, {"&"}, {"*"}, {"("}, {")"}, {"."}, {","}, {":"}, {";"}, {"'"}, {'"'}, {"/"}, {"?"}, {"|"}, {"["}, {"]"}, {"{"}, {"}"}, {"<"}, {">"},};
String passwordSet = "", SSIDset = "", letter = "", text = "";


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
//------preferences init------
Preferences preferences;

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

  //------multicore------
  xTaskCreatePinnedToCore(core1, "core1", 150000, NULL, 1, &Task1, 0);
  delay(500); 

  //------preference------
  preferences.begin("OTSI", false);

  //------read eeprom------
  first = preferences.getInt("first", 0);
  if(first == 0){
    preferences.putInt("nextLvlGoal", 5);
    preferences.putInt("lvl", 0);
  }
  else{
    main_utc = preferences.getInt("timeZone", 0);
    ssid = preferences.getString("ssid");
    password = preferences.getString("wifi_password");
    happines = preferences.getInt("happiness", 0);
    numOfInteractions = preferences.getInt("interactions", 0);
    t_fromStart_firstRead = preferences.getLong("onTime", 0);
    nextLvlGoal = preferences.getInt("nextLvlGoal", 0);
    hp = preferences.getInt("hp", 0);
    lvl = preferences.getInt("lvl", 0);
    sun = preferences.getInt("sun", 0);
    yORnTime = preferences.getString("dst");
    dst = preferences.getInt("SavingTime", 0);
  }
}

void core1( void * pvParameters ){
  for(;;){
    display2.setTextSize(1);
    display2.setTextColor(WHITE); 
    display2.setCursor(0, 0);
      
    //********************OTSI DIALOG WINDOW********************
    while(current_screen == 0) welcome();

    //********************SCROLL MENU********************
    while(current_screen == 1) scrollMenu();

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

      UleveledUp();
    }//end of while for screen 2

    //********************WIFI SCANER********************
    while(current_screen == 3) wifiScanner();

    //********************STOPWATCH********************
    while(current_screen == 10) stopWatch();

    //********************TIMER********************
    while(current_screen == 4) timer();

    //********************STATS********************
    while(current_screen == 5) stats();

    //********************TIME/TEMP********************
    while(current_screen == 6) timeTemp();

    //********************SETTINGS********************
    while(current_screen == 7) settingsVoid();

    //********************TIME SETTINGS********************
    while(current_screen == 9) timeSettings();

    //********************TIMEZONE********************
    while(current_screen == 12) timeZoneSettings();

    //********************WIFI SETTINGS********************
    while(current_screen == 11) wifiSettings();

    //********************DEV TOOLS********************
    while(current_screen == 8) devToolsVoid();

  }//end of for
}

void loop() {
  myRTC.updateTime();
  tc2 = millis();
  display1.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(WHITE); 

  saveStatsToEEprom();

  //********************FIXED TIME********************
  dateConversion();

  //********************STATS-doziness********************
  dozinessVoid();

  //********************STATS-sun********************
  sunlightExpo();

  //********************STATS-lvl********************
  lvlVoid();

  //********************STATS-happines********************
  if(tc2-t_happiness > 45000){
    happines = happines - 2;
    t_happiness = millis();
    if(happines < 0) happines = 0;
  }
  if(happines < 10){
    if(tc2 - t_happiness1 > 500){
      hp = hp - 2;
      t_happiness1 = millis();
    }
  }

  //********************STATS-hp********************
  if(hp < 1) hp = 1;
  else if(hp > 100) hp = 100;

  //********************FACES WHEN STATS********************
  if(hp < 10){
    hpHigh = false;
    boolStats = true;

    //tired af face
    if(tc2-t1 > 500){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_tiredSleep, 128, 64, WHITE);
      display1.display();
    } 
    if(tc2-t1 > 3000){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_tiredSleep2, 128, 64, WHITE);
      display1.display();
      t1 = tc2;
    }
  }
  else if(happines < 30 && happines > 10 && hpHigh && !sleeping){
    boolStats = true;

    //change face to sad
    if(tc2-t1 > 125){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_sad, 128, 64, WHITE);
      display1.display();
    } 
    if(tc2-t1 > 3150){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_sadBlink, 128, 64, WHITE);
      display1.display();
      t1 = tc2;
    }
  }
  else if(happines < 10 && hpHigh && !sleeping){
    boolStats = true;

    //change face to crying
    if(tc2-t1 > 500){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_crying, 128, 64, WHITE);
      display1.display();
    } 
    if(tc2-t1 > 1000){
      display1.clearDisplay();
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_crying2, 128, 64, WHITE);
      display1.display();
      t1 = tc2;
    }
  }
  else if(happines > 30 && hp > 10 && !sleeping){
    boolStats = false;
    hpHigh = true;
  }


  //********************OTSI GRAPHICS********************
  if(hours_NFU >= 6 || hours_NFU < 22)
    sleeping = false;
  if(hours_NFU >= 22 || hours_NFU < 6)
    sleeping = true;

  //------Otsi Cuppa------
  if(!sleeping && !music_on && !active && !love && inDEV && !fuckU && !boolStats){
    display1.drawBitmap(0, 0, (const unsigned char *)otsi_cuppa, 128, 64, WHITE);
    display1.display();
  }
  //------Otsi normal------
  if(!sleeping && !music_on && !active && !love && !inDEV && !fuckU && !boolStats){
    amountOfwakes = 0;
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
  if(sleeping && !music_on && !devTools && !fuckU || active){
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
  if(sleeping && fuckU){
    display1.clearDisplay();
    
    //change faces and print text
    if(tc2-t_fuckU < 2000) display1.drawBitmap(0, 0, (const unsigned char *)otsi_swearing, 128, 64, WHITE);
    else if(tc2-t_fuckU > 2000 && tc2-t_fuckU < 4000) display1.drawBitmap(0, 0, (const unsigned char *)otsi_fuckU, 128, 64, WHITE);
    else if(tc2-t_fuckU > 4000 && tc2-t_fuckU < 8000) display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
    else if(tc2-t_fuckU > 8000) fuckU = false;
    
    display1.display();
  }
  //------Otsi love------
  if(love && !sleeping && !fuckU && !boolStats){
    display1.clearDisplay();
    display1.drawBitmap(0, 0, (const unsigned char *)otsi_love, 128, 64, WHITE);
    display1.display();
    delay(500);
    love = false;
  }

  //********************OTSI LISTENING TO MUSIC********************
  listeningTime = random(5,15);

  if(tc2-t_music_start < 800 && IDKsomeBool){
    startMusic = random(1, 12);
  }
  //------if time------
  if(startMusic == 4 && IDKsomeBool && !sleeping && !boolStats){ //if time for otsi to listen to music
    t_otsi_music = millis();
    tc2 = millis();
    start_music_animAfunction = true;
    IDKsomeBool = false;
  }
  //------start the animation for listeningtime*30s time------
  if(tc2-t_otsi_music < listeningTime*30000 && start_music_animAfunction && !boolStats || music_dev){
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
}