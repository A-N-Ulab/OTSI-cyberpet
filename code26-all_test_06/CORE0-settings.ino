void devToolsVoid(){
  UleveledUp();

  inDEV = true;
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

      numOfInteractions++;
      updateStats = true;
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

      numOfInteractions++;
      updateStats = true;
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

      numOfInteractions++;
      updateStats = true;
	  }
    lastButtonPress = millis();
  }

  //------reset stats to 0------
  if(digitalRead(SW) == LOW && settings1 == 4){
    if (millis() - lastButtonPress > 50) {
      //change values
      first = 0;
      happines = 100;
      numOfInteractions = 0;
      t_fromStart_firstRead = 0;
      nextLvlGoal = 4;
      hp = 100;
      lvl = 0;
      sun = 100;
      otsisName = "";


      //change EEprom
      preferences.putInt("first", first);
      preferences.putInt("happiness", happines);
      preferences.putInt("interactions", numOfInteractions);
      preferences.putLong("onTime", t_fromStart_firstRead);
      preferences.putInt("nextLvlGoal", nextLvlGoal);
      preferences.putInt("hp", hp);
      preferences.putInt("lvl", lvl);
      preferences.putInt("sun", sun);
      preferences.putString("OtsisName", otsisName);
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
    //reset values to 0
    display2.setCursor(10, 34);
    display2.println("Reset stats to 0");
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
      inDEV = false;

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  } 
}

void timeZoneSettings(){
  UleveledUp();

  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
      changeUTC--;
      if(changeUTC < 0) changeUTC = 24;
      main_utc = times[changeUTC+dst];
    }
    else {
      changeUTC++;
      if(changeUTC > 24) changeUTC = 0;
      main_utc = times[changeUTC+dst];
    }
    if(main_utc > 0) plus = "+";
    else plus = "";

    display2.setCursor(5, 5);
    display2.print("TimeZone: UTC"+plus+String(main_utc));
    display2.setCursor(5, 15);
    display2.print("City: "+String(list_of_timeZones[main_utc+11-dst]));

    display2.setCursor(5, 25);
    display2.print("3 - DST: ");
    display2.print(yORnTime);
    display2.display();
    
  }
  lastStateCLK = currentStateCLK;
  delay(1);


  //------Daylight saving time------
  if(digitalRead(BT1) == LOW){
    if(millis() - lastButtonPress > 50) {
      fix_6();
      if(dstBool){
        yORnTime = "N";
        dst = 0;
      }
      else{
        yORnTime = "Y"; 
        dst = 1;
      }
      dstBool = !dstBool;
      preferences.putString("dst", yORnTime);
      preferences.putInt("SavingTime", dst);
      
      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }

  //------Return button------
  //to time settings
  if(digitalRead(BT3) == LOW){
    if(millis() - lastButtonPress > 50) {
      fix_1();
      current_screen = 9;
      preferences.putInt("timeZone", main_utc);

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
}


//**********************************************************************


void timeSettings(){
  UleveledUp();

  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
  //------Set Time zone------
  display2.setCursor(5,5);
  display2.print("2 - Set TimeZone");
  if(digitalRead(BT2) == LOW){
    if(millis() - lastButtonPress > 50){
      fix_6();
      current_screen = 12;

      numOfInteractions++;
      updateStats = true;
    }
  	lastButtonPress = millis();
  }
  //------Update via wifi------
  display2.setCursor(5,15);
  display2.print("3 - Update via WiFi");
  if(digitalRead(BT1) == LOW){
    if(millis() - lastButtonPress > 50){
      WiFiupdateTime();
    
      numOfInteractions++;
      updateStats = true;
    }
    lastButtonPress = millis();
  }
  //------Last updated------
  display2.setCursor(5,35);
  display2.print("Last time update:");
  display2.setCursor(5,45);
  display2.print(LastUpdated);
       
  //------Return button------
  //to settings
  if(digitalRead(BT3) == LOW){
    if(millis() - lastButtonPress > 50) {
      fix_4();
      current_screen = 7;

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
  display2.display();
}


//**********************************************************************


void settingsVoid(){
  UleveledUp();
  display2.clearDisplay();

  currentStateCLK = digitalRead(CLK);
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
    display2.println("Time and date");
    display2.setCursor(10, 15);
    display2.println("Wifi");
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
  if(digitalRead(SW) == LOW && settings == 3 && devTools){
    if (millis() - lastButtonPress > 50){
      fix_2();
      current_screen = 8;

      numOfInteractions++;
      updateStats = true;
	  }
  	lastButtonPress = millis();
  }
  //to time settings
  else if(digitalRead(SW) == LOW && settings == 1){
    if (millis() - lastButtonPress > 50){
      fix_2();
      current_screen = 9;

      numOfInteractions++;
      updateStats = true;
	  }
  	lastButtonPress = millis();
  }
  //to wifi settigs
  else if(digitalRead(SW) == LOW && settings == 2){
    if (millis() - lastButtonPress > 50){
      fix_2();
      current_screen=11;

      numOfInteractions++;
      updateStats = true;
	  }
  	lastButtonPress = millis();
  }
  //------Return button------
  if(digitalRead(BT3) == LOW){
    if (millis() - lastButtonPress > 50) {
      fix_1();
      current_screen = 1;

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
}


//**********************************************************************


void chagngeWiFi(){
  if(homeWiFi){
    ssid = ssidHome;
    password = passwordHome;
  }
  else{
    ssid = preferences.getString("ssid");
    password = preferences.getString("wifi_password");
  }
}