void timeTemp(){
  UleveledUp();

  //------date and time convertion------
  String date = String(day_NFU) + "/" + String(mon_NFU) + "/" + String(year_NFU);
  String time = String(hours_NFU) + ":" +  minutes_NFU + ":" + seconds_NFU;
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

      numOfInteractions++;
      updateStats = true;
	  } 
  	lastButtonPress = millis();
  }
}

void stats(){
  UleveledUp();

  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE); 
  //------Stats------
  //happiness
  display2.setCursor(5,5);
  display2.print("happiness:");
  display2.setCursor(105,5);
  display2.print(happines);
  //hp
  display2.setCursor(5,15);
  display2.print("hp:");
  display2.setCursor(105,15);
  display2.print(hp);
  //doziness
  display2.setCursor(4,25);
  display2.print("doziness:");
  display2.setCursor(105,25);
  display2.print(doziness);
  //sunlight
  display2.setCursor(5,35);
  display2.print("sunlight:");
  display2.setCursor(105,35);
  display2.print(sun);
  //lvl
  display2.setCursor(5,45);
  display2.print("lvl:");
  display2.setCursor(105,45);
  display2.print(lvl);
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