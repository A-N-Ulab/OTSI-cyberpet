#include <Preferences.h> 
Preferences preferences;

void setup() {
  Serial.begin(9600);
  preferences.begin("myfile", false);

  int readId = preferences.getInt("boardId", 0); 
  Serial.print("Read Id = ");
  Serial.println(readId);

  float readParam = preferences.getFloat("param", 0); 
  Serial.print("Read param = ");
  Serial.println(readParam);
  preferences.end();

}

void loop() {
}
