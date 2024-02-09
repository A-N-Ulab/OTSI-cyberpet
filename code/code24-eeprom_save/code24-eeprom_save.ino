#include <Preferences.h> 
Preferences preferences;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("Initialize System"));
  int boardId = 18; 
  float param = 26.5;
  
  preferences.begin("myfile", false);
  //preferences.clear(); // remove all preferences in namespace myfile
  //preferences.remove("boardId");// remove varname in the namespace

  preferences.putInt("boardId", boardId);
  preferences.putFloat("param", param);

  preferences.end();
    
}

void loop() {}
