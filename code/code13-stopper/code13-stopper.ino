bool stoper_start = true, lap = false;
int seconds = 0, minutes = 0, seconds_lap = 0, minutes_lap = 0;
long t_stoper;


long tc1;


void setup() {
  Serial.begin(9600);
  t_stoper = millis();

}

void loop() {
  tc1 = millis();
  if(stoper_start){
    if(tc1-t_stoper > 1000){
      seconds++;
      if(seconds == 60){
        minutes++;
        seconds = 0;
      }
      //lap
      if(lap){
        seconds_lap = seconds;
        minutes_lap = minutes;
        seconds = 0;
        minutes = 0;
        lap = false;
      }

      //debug
      if(seconds == 57)
        lap = true;
      
      Serial.print("minutes: ");
      Serial.print(minutes);
      Serial.print("\t");
      Serial.print("seconds: ");
      Serial.println(seconds);

      Serial.print("Lap minutes: ");
      Serial.print(minutes_lap);
      Serial.print("\t");
      Serial.print("Lap seconds: ");
      Serial.println(seconds_lap);

      t_stoper = tc1;
    }
  }
}
