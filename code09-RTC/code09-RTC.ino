#include <virtuabotixRTC.h>
#include <Time.h>

virtuabotixRTC myRTC(40, 39, 34);

void setup()  {
  Serial.begin(9600);
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(45, 29, 13, 1, 23, 10, 2023);
}
void loop()  {
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