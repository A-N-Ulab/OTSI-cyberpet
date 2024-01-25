#include <WiFi.h>
#include "time.h"
#include <virtuabotixRTC.h>

virtuabotixRTC myRTC(40, 39, 34);

const char* ssid = "UPC0520860";
const char* password = "Ae8kdjbmaerx";
const char* ntpServer = "pool.ntp.org";
long  gmtOffset_sec = 3600 * 1, t_WT;
int  daylightOffset_sec = 0;
String ISconnected = "";
String WiFitime = "";
int Ahour, Amin, Asec, Aday, Amonth, Ayear, Awday;




void setup()
{
  Serial.begin(9600);
  WiFiupdateTime();
}

void loop(){
  myRTC.updateTime();
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print(myRTC.dayofweek);
  Serial.print(" ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);
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
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
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