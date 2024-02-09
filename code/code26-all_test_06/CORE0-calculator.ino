void calculator(){
  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)calcBG, 128, 64, WHITE);
  display2.display();

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