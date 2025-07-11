int main_utc = 4;
int settingsT = 0, hourPM, dayPM, monPM, yearPM, mon_NFU, day_NFU, year_NFU;
int years[] = {2024, 2028, 2032, 2036, 2040, 2044, 2048, 2052, 2056, 2060, 2064, 2068, 2072, 2076, 2080, 2084, 2088, 2092, 2096, 2100};


int hT, mT, sT, dT, mmT, yT;
String minutes_NFU = "", hours_NFU = "", seconds_NFU = "";
#include <virtuabotixRTC.h>
virtuabotixRTC myRTC(40, 39, 34);

void setup() {
  Serial.begin(9600);
  hT = 20;
  mT = 50;
  sT = 0;
  dT = 31;
  mmT = 1;
  yT = 2024;
}




void loop() {
  //********************FIXED TIME******************
  //------Fixed hours, minutes, seconds------
  //hours
  int h = hT + main_utc;
  if(h > 23){
    hours_NFU = h - 24;
    dayPM = 1;
  }
  else if(h < 0){
    hours_NFU = 24 + h;
    dayPM = -1;
  }
  else{
    hours_NFU = h;
    dayPM = 0;
  }
  //minutes
  int m = mT;
  if(m < 10){
    minutes_NFU = "";
    minutes_NFU += "0";
    minutes_NFU += String(m);
  }
  else minutes_NFU = String(m);
  //seconds
  int s = sT;
  if(s < 10){
    seconds_NFU = "";
    seconds_NFU += "0";
    seconds_NFU += String(s);
  }
  else seconds_NFU = String(s);

  //------Fixed date------
  //mon_NFU, day_NFU, year_NFU, monPM, yearPM;
  int d = dT;
  int mm = mmT;
  int y = yT;

  //day
  if(d+dayPM > 0 && d+dayPM < 29){
    day_NFU = d + dayPM;
    monPM = 0;
  }
  else if(d+dayPM == 0){
    monPM = -1;
    if(mm == 1 || mm == 2 || mm == 4 || mm == 6 || mm == 8 || mm == 9 || mm == 11) day_NFU = 31;
    else if(mm == 3){
      if(y == years[1] || y == years[2] || y == years[3] || y == years[4] || y == years[5] || y == years[0] || y == years[6] || y == years[7] || y == years[8] || y == years[9] || y == years[10]) day_NFU = 29;
      else day_NFU = 28;
    }
    else day_NFU = 30;
  }
  else if(d+dayPM == 29){
    if(y == years[1] || y == years[2] || y == years[3] || y == years[4] || y == years[5] || y == years[0] || y == years[6] || y == years[7] || y == years[8] || y == years[9] || y == years[10]){
      if(mm == 2) day_NFU = d+dayPM;
      else day_NFU = d+dayPM;
    }
    else if(y != years[1] || y != years[2] || y != years[3] || y != years[4] || y != years[5] || y != years[0] || y != years[6] || y != years[7] || y != years[8] || y != years[9] || y != years[10]){
      if(mm == 2){
        monPM = 1;
        day_NFU = 1;
      }
      else day_NFU = d+dayPM;
    } 
  }
  else if(d+dayPM == 30){
    if(y == years[1] || y == years[2] || y == years[3] || y == years[4] || y == years[5] || y == years[0] || y == years[6] || y == years[7] || y == years[8] || y == years[9] || y == years[10]){
      if(mm == 2){
        monPM = 1;
        day_NFU = 1;
      }
      else day_NFU = d+dayPM;
    }
    else day_NFU = d+dayPM;
  }
  else if(d+dayPM == 31){
    if(mm == 4 || mm == 6 || mm == 9 || mm == 11){
      monPM = 1;
      day_NFU = 1;
    } 
    else day_NFU = d+dayPM;
  }
  else if(d+dayPM == 32){
    monPM = 1;
    day_NFU = 1;
  }
  //mon
  if(mm+monPM > 0 && mm+monPM < 13){
    mon_NFU = mm+monPM;
    yearPM = 0;
  }
  else if(mm+monPM == 0){
    mon_NFU = 12;
    yearPM = -1;
  }
  else if(mm+monPM == 13){
    mon_NFU = 1;
    yearPM = 1;
  }
  //year
  year_NFU = y + yearPM;



  //debug
  Serial.print("h: ");
  Serial.print(hours_NFU);
  Serial.print("\t");
  Serial.print("m: ");
  Serial.print(minutes_NFU);
  Serial.print("\t");
  Serial.print("s: ");
  Serial.print(seconds_NFU);
  Serial.print("\t");
  Serial.print("D: ");
  Serial.print(day_NFU);
  Serial.print("\t");
  Serial.print("MM: ");
  Serial.print(mon_NFU);
  Serial.print("\t");
  Serial.print("Y: ");
  Serial.println(year_NFU);
  delay(5000);
}
