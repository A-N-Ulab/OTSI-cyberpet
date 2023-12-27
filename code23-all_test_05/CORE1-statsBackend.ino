void dozinessVoid(){
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
    if(minutes_NFU_int == 30 && seconds_NFU_int == 0 || DEVyawn_on){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }
  if(doziness == 80){
    if(minutes_NFU_int == 20 && seconds_NFU_int == 0 || minutes_NFU_int == 40 && seconds_NFU_int == 0){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }
  if(doziness == 90){
    if(minutes_NFU_int == 15 && seconds_NFU_int == 0 || minutes_NFU_int == 30 && seconds_NFU_int == 0 || minutes_NFU_int == 45 && seconds_NFU_int == 0 || minutes_NFU_int == 59 && seconds_NFU_int == 0){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }
  if(doziness == 100){
    if(minutes_NFU_int == 10 && seconds_NFU_int == 0 || minutes_NFU_int == 20 && seconds_NFU_int == 0 || minutes_NFU_int == 30 && seconds_NFU_int == 0 || minutes_NFU_int == 40 && seconds_NFU_int == 0 || minutes_NFU_int == 50 && seconds_NFU_int == 0 || minutes_NFU_int == 59 && seconds_NFU_int == 0){
      display1.drawBitmap(0, 0, (const unsigned char *)otsi_yawing, 128, 64, WHITE);
      display1.display();
      delay(1000);
    }
  }
}

void lvlVoid(){
  //time fromm first boot
  t_fromStart = (millis()/1000) + t_fromStart_firstRead;

  //leveling up
  calcLvl = (t_fromStart/60) + numOfInteractions;

  if(calcLvl == nextLvlGoal){
    lvlUp = true;
    lvl++;
    nextLvlGoal = nextLvlGoal*1.5;
    preferences.putInt("nextLvlGoal", nextLvlGoal);
    preferences.putInt("lvl", lvl);
  }
}

void UleveledUp(){
  if(lvlUp){
    display2.clearDisplay();
    display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE); 
    display2.setCursor(5,10);
    display2.print("OTSI HAS LEVELED UP");
    display2.setCursor(5,20);
    display2.print("OTSI's lvl: ");
    display2.print(lvl);
    display2.display();
    delay(3000);
    lvlUp = false;
  }
}

void sunlightExpo(){
  int sunRead;
  sunRead = analogRead(7);

  if(sunRead > 1000){
    if(tc2 - t_sun > 10000){
      sun--;
      if(sun < 0) sun = 0;
      t_sun = millis();
    }
  }
  else{
    if(tc2 - t_sun1 > 5000){
      sun++;
      if(sun > 100) sun = 100;
      t_sun1 = millis();
    }
  }

  if(sun < 20){
    if(tc2-t_sun_1 > 5000){
      hp = hp - 2;
      t_sun_1 = tc2;
    }
  }
  else{
    if(tc2-t_sun_2 > 10000){
      hp = hp + 2;
      t_sun_2 = tc2;
    }
  }
}

void saveStatsToEEprom(){
  if(tc2 - t_save > 5000 || updateStats){
    preferences.putInt("interactions", numOfInteractions);
    preferences.putLong("onTime", t_fromStart);
    preferences.putInt("sun", sun);
    preferences.putInt("hp", hp);
    preferences.putInt("happiness", happines);
    t_save = millis();
    updateStats = false;
  }
}