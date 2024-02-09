void welcome(){
  UleveledUp();

  tc1 = millis();
  
  //------When first turn on------
  if(first == 0){
    //choosing name
    while(!nameChoosen){
      askForName();
      nameChoosen = true;
      preferences.putBool("nameChoosen", nameChoosen);
    }
    //delay(1000);

    //frame & buttons description
    display2.clearDisplay();
    display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
    display2.drawBitmap(5, 55, (const unsigned char *)ok, 16, 8, WHITE);

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
    if (digitalRead(BT3) == LOW){
	    if (millis() - lastButtonPress > 50){
        first++;
        preferences.putInt("first", first);

        numOfInteractions++;
        updateStats = true;
      }
      lastButtonPress = millis();
	  }  
  }
  //------When second and more turn on------
  else if(first != 0){
    //------frames------
    display2.clearDisplay();
    display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
    if(sleeping) display2.drawBitmap(5, 54, (const unsigned char *)interact, 40, 8, WHITE);  
    else{
      display2.drawBitmap(5, 55, (const unsigned char *)pet, 24, 8, WHITE);
      display2.drawBitmap(30, 54, (const unsigned char *)interact, 40, 8, WHITE);  
    }

    //------fuckU------
    if(sleeping){
      if(digitalRead(BT2) == LOW){
        if(millis() - lastButtonPress > 50){
          amountOfwakes++;
          t_fuckU = millis();
          t_fuckU1 = millis();
          fuckU = true;

          numOfInteractions++;
          updateStats = true;
        }
        lastButtonPress = millis();
      }
    }

    //------pet------
    if(digitalRead(BT1) == LOW){
      if(millis() - lastButtonPress > 50){
        if(tc1-t_love > 6000){
          love = true;
          if(happines < 92) happines = happines + 8;
          else if(happines >= 92) happines = 100;
          t_love = tc1;

          numOfInteractions++;
          updateStats = true;
        }
      }
      lastButtonPress = millis();
    }

    //------when listening to music------
    if(digitalRead(BT2) == LOW && start_music_animAfunction){
      if(millis() - lastButtonPress > 50){
        song = true;
        t_music_titles = millis();
        randomMusic = random(0, 19);
        if(happines < 95) happines = happines + 5;
        else if(happines >= 95) happines = 100;

        numOfInteractions++;
        updateStats = true;
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
        
  }

  //fuckU text
  if(fuckU){
    if(tc1-t_fuckU1 > 0 && tc1-t_fuckU1 < 4000){
      display2.clearDisplay();
      display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
      display2.drawBitmap(5, 54, (const unsigned char *)interact, 40, 8, WHITE);  
      display2.setCursor(0,5);
      if(amountOfwakes < 1){
        display2.print(" YOU SON*******\n");
        display2.print(" Why are you waking\n");
        display2.print(" me up?!");
      }
      else{
        display2.print(" YOU SON*******\n");
        display2.print(" Why are you waking\n");
        display2.print(" me up again?!");
      }
    }
    if(tc1-t_fuckU1 > 4000){
      display2.clearDisplay();
      display2.drawBitmap(0, 0, (const unsigned char *)frames, 128, 64, WHITE);  
      display2.drawBitmap(5, 54, (const unsigned char *)interact, 40, 8, WHITE);  
      display2.setCursor(0,5);
      if(amountOfwakes < 4){
        display2.print(" Anyway...\n");
        display2.print(" I'm going back to\n");
        display2.print(" sleep. Dont you\n");
        display2.print(" dare wake me up\n");
        display2.print(" again!\n");
      }
      else if(amountOfwakes >= 4 && amountOfwakes < 10){
        display2.print(" thats ");
        display2.print(amountOfwakes);
        display2.print("th time!\n");
        display2.print(" You wake me up again\n");
        display2.print(" AND YOU ARE DEAD\n");
      }
      else{
        display2.print(" ok fine, \n");
        display2.print(" WHAT THE FUCK\n");
        display2.print(" DO YOU WANT?!\n");
      }
    }
  }

  //------display on display2------
  display2.display();

  //------To main menu------
  if(digitalRead(SW) == LOW){
    if (millis() - lastButtonPress > 50) {
      fix_2();
      current_screen = 1;

      numOfInteractions++;
      updateStats = true;
		}
		lastButtonPress = millis();
  }
}

void scrollMenu(){
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
    if(item_sel_previous < 0) item_sel_previous = NUM_ITEMS - 1;
    item_sel_next = item_selected + 1;  
    if(item_sel_next >= NUM_ITEMS) item_sel_next = 0;
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
    if (millis() - lastButtonPress > 50){
      dev_activate++;

      numOfInteractions++;
      updateStats = true;
    }
    lastButtonPress = millis();
  }

  if(tc1-t_activate_dev > 3000){
    dev_activate = 0;
    t_activate_dev = millis();
  }
  if(dev_activate >= 10) devTools = true;
  //------Main menu navigation------
  //to Wifi scanner
  else if(digitalRead(SW) == LOW && item_selected == 4){
    if (millis() - lastButtonPress > 50) {
      fix_2();
      current_screen = 3;
      t_s2 = millis();
      first_WiFi = true;
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();

      numOfInteractions++;
      updateStats = true;
		}
		lastButtonPress = millis();
  }
  //to Timer
  else if(digitalRead(SW) == LOW && item_selected == 2){
    if (millis() - lastButtonPress > 50) {
      fix_3();
      current_screen = 4;

      numOfInteractions++;
      updateStats = true;
		}
		lastButtonPress = millis();
  }
  //to Stats
  else if(digitalRead(SW) == LOW && item_selected == 5){
    if (millis() - lastButtonPress > 50) {
      fix_2();
      current_screen = 5;

      numOfInteractions++;
      updateStats = true;
		}
		lastButtonPress = millis();
  }
  //to Time&Temp
  else if(digitalRead(SW) == LOW && item_selected == 0){
    if (millis() - lastButtonPress > 50) {
      fix_2();
      current_screen = 6;

      numOfInteractions++;
      updateStats = true;
		}
		lastButtonPress = millis();
  }
  //to Settings
  else if(digitalRead(SW) == LOW && item_selected == 6){
    if (millis() - lastButtonPress > 50) {
      fix_2();
      current_screen = 7;

      numOfInteractions++;
      updateStats = true;
		}
		lastButtonPress = millis();
  }
  //to Stopwatch
  else if(digitalRead(SW) == LOW && item_selected == 3){
    if (millis() - lastButtonPress > 50) {
      fix_2();
      current_screen = 10;

      numOfInteractions++;
      updateStats = true;
		}
    lastButtonPress = millis();
  }
  //to calculator
  else if(digitalRead(SW) == LOW && item_selected == 1){
    if (millis() - lastButtonPress > 50) {
      fix_2();
      current_screen = 13;

      numOfInteractions++;
      updateStats = true;
		}
    lastButtonPress = millis();
  }
  //------Return button------
  if(digitalRead(BT3) == LOW){
    if (millis() - lastButtonPress > 50) {
      current_screen = 0;

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
}

void askForName(){
  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);

  //------Insert letter to "text"------
  if(digitalRead(SW) == LOW){
    if(millis() - lastButtonPress > 50){
      if(counterLetterName <= 5){
        otsisName = otsisName + letterName;
        counterLetterName++;
      }
      fix_5();
    }
    lastButtonPress = millis();
  }

  //------backspace------
  if(digitalRead(BT2) == LOW){
    if(millis() - lastButtonPress > 50){
      otsisName = otsisName.substring(0, otsisName.length()-1);
      if(counterLetterName >= 0) counterLetterName--;
      fix_5();
    }
    lastButtonPress = millis();
  }

  if(digitalRead(BT1) == LOW){
    if(millis() - lastButtonPress > 50){
      nameChoosen = true;
      preferences.putString("OtsisName", otsisName);
    }
    lastButtonPress = millis();
  }

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
    display2.setCursor(5,25);
    display2.print("LETTER: ");
    display2.setCursor(50,25);
    //Capital letter or not
    if(digitalRead(BT3) == LOW){
      if(millis() - lastButtonPress > 50){
          display2.print(list_of_C_letters[letterIndex]);
          letterName = list_of_C_letters[letterIndex];
      }
    lastButtonPress = millis();
    }
    else{ 
      display2.print(list_of_nonC_letters[letterIndex]);
      letterName = list_of_nonC_letters[letterIndex];
    }
    //print written text on a screen
    display2.setCursor(5, 35);
    display2.print("NAME: ");
    display2.setCursor(35, 35);
    display2.print(otsisName);

    display2.setCursor(5, 5);
    display2.print("Set your OTSI's name");
    display2.setCursor(6, 15);
    display2.print("max 6 characters");

    display2.display(); 
  } 
  lastStateCLK = currentStateCLK;
  delay(1);  
}