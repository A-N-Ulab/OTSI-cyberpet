void timer(){
  UleveledUp();

  currentStateCLK = digitalRead(CLK);
  //------start button------
  if(digitalRead(SW) == LOW && current_screen == 4){
    if (millis() - lastButtonPress > 50) {
      time_for = setting * 60;
      button = time_for;

      numOfInteractions++;
      updateStats = true;
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
    display2.drawBitmap(0, 0, (const unsigned char *)frame_timer, 128, 64, WHITE);
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
    display2.drawBitmap(0, 0, (const unsigned char *)frame_timer, 128, 64, WHITE);
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
    display2.setCursor(85, 32);
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

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
}

void stopWatch(){
  UleveledUp();

  tc1 = millis();
  //------encoder start, lap start------
  if(digitalRead(SW) == LOW){
    if (millis() - lastButtonPress > 50) {
      if(stopState){
        stoper_start = false;
      }
      else{
        if(seconds != 0 || minutes != 0){
          stoper_start = true;
        }
        else{
          stoper_start = true;
          seconds = 0;
          minutes = 0;
        }
        
      }
      stopState = !stopState;

      numOfInteractions++;
      updateStats = true;
    }
		lastButtonPress = millis();
  }
  if(digitalRead(BT1) == LOW){
    if(millis() - lastButtonPress > 50){
      seconds = 0;
      minutes = 0;
    }
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
  display2.drawBitmap(0, 0, (const unsigned char *)frame_stopwatch, 128, 64, WHITE);
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

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
}