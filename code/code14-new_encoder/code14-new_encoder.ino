#include <SimpleRotary.h>

// Pin A, Pin B, Button Pin
SimpleRotary rotary(19,20,26);

int dumb;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  byte i;

  // 0 = not turning, 1 = CW, 2 = CCW
  i = rotary.rotate();

  if ( i == 1 ) {
    Serial.println("cw");
  }

  if ( i == 2 ) {
    Serial.println("ccw");
  }
  
  if(rotary.push() == HIGH){
    Serial.println("pressed");
  }

}
