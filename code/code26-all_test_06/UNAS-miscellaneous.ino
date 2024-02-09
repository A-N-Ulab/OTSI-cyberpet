void fix_1(){
  currentStateCLK = 1;
  lastStateCLK = 0;
}
void fix_2(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  item_selected--;
}
void fix_3(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  setting--;
  item_selected--;
}
void fix_4(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  settings1--;
}
void fix_5(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  letterIndex--;
}
void fix_6(){
  currentStateCLK = 1;
  lastStateCLK = 0;
  main_utc--;
}

void batteryVoltage(){
  readBat = analogRead(5);
  batV = (readBat*6.6) / 4095.0;
  batVstring = String(batV) + "V";
  display1.setCursor(90, 54);
  display1.print(batVstring);
}