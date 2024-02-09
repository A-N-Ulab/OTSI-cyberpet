void WiFiupdateTime(){
  t_WT = millis();
  //------connect to WiFi------
  WiFi.begin(ssid, password);
  //------check the connection------
  while (WiFi.status() != WL_CONNECTED && millis()-t_WT < 5000) delay(500);
  if(WiFi.status() != WL_CONNECTED && millis()-t_WT > 5000) ISconnected = false;
  else ISconnected = true;
  //------init and get the time------
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) WiFitime = "failed";
  //------disconect from WiFi------
  WiFi.disconnect();
  //------set RTC------
  if(ISconnected){
    Ahour = timeinfo.tm_hour;
    Amin = timeinfo.tm_min;
    Asec = timeinfo.tm_sec;
    Aday = timeinfo.tm_mday;
    Amonth = timeinfo.tm_mon+1;
    Ayear = timeinfo.tm_year+1900;
    Awday = timeinfo.tm_wday;
    myRTC.setDS1302Time(Asec, Amin, Ahour, Awday, Aday, Amonth, Ayear);
    if(Ayear == 2130 && Ahour == 0 && Amin == 0) LastUpdated = "Failed - try again";
    else LastUpdated = String(Ahour+main_utc) + ":" + String(Amin) + ":" + String(Asec) + " " + String(Aday) + "/" + String(Amonth) + "/" + String(Ayear);
  }
  else LastUpdated = "Failed";
}

void wifiSettings(){
  UleveledUp();

  //------Display init and frames------
  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frame_password, 128, 64, WHITE);

  //------Insert letter to "text"------
  if(digitalRead(SW) == LOW){
    if(millis() - lastButtonPress > 50){
      text = text + letter;
      fix_5();

      numOfInteractions++;
      updateStats = true;
    }
    lastButtonPress = millis();
  }

  //------Switch bteween password and ssid------
  if(digitalRead(BT1) == LOW){
    if(millis() - lastButtonPress > 50){
      changePorS++;
      text = "";
      if(changePorS > 1) changePorS = 0;
      fix_5();

      numOfInteractions++;
      updateStats = true;
    } 
    lastButtonPress = millis();
  }
  if(changePorS == 0) passwordSet = text;
  else if(changePorS == 1) SSIDset = text;

  //------Encoder based keyboard------
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
      letterIndex--;
      if(letterIndex < 0) letterIndex = 60;
    }
    else {
      letterIndex++;
      if(letterIndex > 60) letterIndex = 0;
    }
    display2.setCursor(5,5);
    display2.print("letter: ");
    display2.setCursor(47,5);
    //Capital letter or not
    if(digitalRead(BT2) == LOW){
      if(millis() - lastButtonPress > 50){
          display2.print(list_of_C_letters[letterIndex]);
          letter = list_of_C_letters[letterIndex];
      }
    lastButtonPress = millis();
    }
    else{ 
      display2.print(list_of_nonC_letters[letterIndex]);
      letter = list_of_nonC_letters[letterIndex];
    }
    //print written text on a screen
    display2.setCursor(5, 25);
    display2.print("PW: ");
    display2.setCursor(24,25);
    display2.print(passwordSet);
    display2.setCursor(5,35);
    display2.print("ID:");
    display2.setCursor(24,35);
    display2.print(SSIDset);

    display2.display(); 
  } 
  lastStateCLK = currentStateCLK;
  delay(1);  

  //------return button------
  if(digitalRead(BT3) == LOW){
    if(millis() - lastButtonPress > 50) {
      //ask if save
      while(waitWIFIsettings){
        display2.clearDisplay();
        display2.setCursor(5,5);
        display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
        display2.print("Do you want to save \n it?");
        display2.setCursor(5,25);
        display2.print("1: Yes");
        display2.setCursor(5,35);
        display2.print("2: No");
        display2.display();

        //yes
        if(digitalRead(BT1) == LOW){
          if(millis() - lastButtonPress > 50){
            waitWIFIsettings = false;

            preferences.putString("ssid", SSIDset);
            preferences.putString("wifi_password", passwordSet);
            ssidOffgrid = SSIDset;
            passwordOffgrid = passwordSet;
            display2.clearDisplay();
            display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
            display2.setCursor(5,5);
            display2.print("Saved");
            display2.display();
            delay(1000);
            display2.clearDisplay();
            current_screen = 11;
          }
          lastButtonPress = millis();
        }

        //no
        else if(digitalRead(BT2) == LOW){
          if(millis() - lastButtonPress > 50){
            waitWIFIsettings = false;
            display2.clearDisplay();
            display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
            display2.setCursor(5,5);
            display2.print("Not saved");
            display2.display();
            delay(1000);
            display2.clearDisplay();
            current_screen = 11;
          }
          lastButtonPress = millis();
        }
      }
      

      fix_4();
      numOfInteractions++;
      updateStats = true;
      waitWIFIsettings = true;
	  } 
  lastButtonPress = millis();
  }
}

void wifiScanner(){
  t_s1 = millis();
  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
  //------Scan WiFi and print to the screen------
  if(t_s1-t_s2 > 10000 || first_WiFi){
    first_WiFi = false;
    int n = WiFi.scanNetworks();
    if (n != 0){
      for (int i = 0; i < n; i++){
        switch (WiFi.encryptionType(i)){
          case WIFI_AUTH_OPEN:
            WiFi_scan_encryption = "open";
            break;
          case WIFI_AUTH_WEP:
            WiFi_scan_encryption = "WEP";
            break;
          case WIFI_AUTH_WPA_PSK:
            WiFi_scan_encryption = "WPA";
            break;
          case WIFI_AUTH_WPA2_PSK:
            WiFi_scan_encryption = "WPA2";
            break;
          case WIFI_AUTH_WPA_WPA2_PSK:
            WiFi_scan_encryption = "WPA+WPA2";
            break;
          case WIFI_AUTH_WPA2_ENTERPRISE:
            WiFi_scan_encryption = "WPA2-EAP";
            break;
          case WIFI_AUTH_WPA3_PSK:
            WiFi_scan_encryption = "WPA3";
            break;
          case WIFI_AUTH_WPA2_WPA3_PSK:
            WiFi_scan_encryption = "WPA2+WPA3";
            break;
          case WIFI_AUTH_WAPI_PSK:
            WiFi_scan_encryption = "WAPI";
            break;
          default:
            WiFi_scan_encryption = "unknown";
          }
        WiFi_scan_len = String(WiFi.SSID(i).c_str()).length();
        WiFi_enc_len = WiFi_scan_encryption.length();
        max_len = 15 - WiFi_enc_len;
        if(WiFi_scan_len > max_len) WiFi_scan_SSID = String(WiFi.SSID(i).c_str()).substring(0, max_len) + "... " + WiFi_scan_encryption;
        else WiFi_scan_SSID = String(WiFi.SSID(i).c_str()) + "... " + WiFi_scan_encryption;
        if(n <= 5){
          display2.setCursor(5, (10*(i+1))-5);
          display2.print(WiFi_scan_SSID);
          display2.display();
        }
        else{
          display2.setCursor(5, 5);
          display2.print("Too many networks");
          display2.display();
        }
      }
    }
    t_s2 = t_s1;
  }
  WiFi.scanDelete();

  //------Return button------
  if(digitalRead(BT3) == LOW){
    if(millis() - lastButtonPress > 50) {
      fix_1();
      current_screen = 1;

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
}


void wifiMainSettings(){
  UleveledUp();

  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
  display2.setCursor(5,3);
  display2.print("1: set password and \n ssid");
  display2.setCursor(5,23);
  display2.print("2: change WiFi to");
  display2.setCursor(5,33);
  display2.print(whichWiFi);

  display2.setCursor(5, 43);
  display2.println("PW: ");
  display2.setCursor(24, 43);
  display2.println(password);
  display2.setCursor(5, 53);
  display2.println("ID: ");
  display2.setCursor(24, 53);
  display2.println(ssid);
  display2.display();

  //change used wifi
  if(digitalRead(BT2) == LOW){
    if(millis() - lastButtonPress > 50) {
      if(homeWiFibutton){
        homeWiFi = true;
        whichWiFi = "Offgrid Wifi";
        preferences.putBool("homeWiFi", homeWiFi);
        preferences.putString("whichWiFi", whichWiFi);
      }
      else{
        homeWiFi = false;
        whichWiFi = "Home WiFi";
        preferences.putBool("homeWiFi", homeWiFi);
        preferences.putString("whichWiFi", whichWiFi);
      }
      homeWiFibutton = !homeWiFibutton;
    } 
    lastButtonPress = millis();
  }

  //set password
  if(digitalRead(BT1) == LOW){
    if(millis() - lastButtonPress > 50){
      passwordSet = "";
      SSIDset = "";
      current_screen = 15;

      fix_2();
      numOfInteractions++;
      updateStats = true;
    }
    lastButtonPress = millis();
  }
  
  //return
  if(digitalRead(BT3) == LOW){
    if(millis() - lastButtonPress > 50) {
      current_screen = 7;

      fix_4();
      numOfInteractions++;
      updateStats = true;
    } 
    lastButtonPress = millis();
  }
}