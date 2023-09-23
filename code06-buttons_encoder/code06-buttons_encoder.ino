//------libraries------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//------Displays init------
Adafruit_SSD1306 display1(128, 64, &Wire, 21, 500000UL);
Adafruit_SSD1306 display2(128, 64, &Wire1, -1, 400000UL);

//------pins define------
#define CLK 20
#define DT 19
#define SW 26
#define BT1 48
#define BT2 47
#define BT3 33

//------variables------
int counter = 0;
int currentStateCLK;
int lastStateCLK;
unsigned long lastButtonPress = 0;

void setup(){
  //------serial port------
  Serial.begin(9600);

  //------pinmodes------
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
	
  //------encoder read------
	lastStateCLK = digitalRead(CLK);

  //------Built-in and 0.96' display setup------
  Wire.begin(17,18);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display1.clearDisplay();
  display1.display();

  Wire1.begin(41,42);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display2.clearDisplay();
  display2.display();
}

void loop(){
  //------encoder------
	currentStateCLK = digitalRead(CLK);
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
		} 
    else {
			counter ++;
		}
    //to do while rotating

	}
  lastStateCLK = currentStateCLK;

  //------button encoder------
	int btnStateE = digitalRead(SW);
	if (btnStateE == LOW) {
		if (millis() - lastButtonPress > 50) {
			//to do when pressed
		}
		lastButtonPress = millis();
	}

  //------button 1------
	int btnState1 = digitalRead(BT1);
	if (btnState1 == LOW) {
		if (millis() - lastButtonPress > 50) {
			//to do when pressed
		}
		lastButtonPress = millis();
	}

  //------button 2------
	int btnState2 = digitalRead(BT2);
	if (btnState2 == LOW) {
		if (millis() - lastButtonPress > 50) {
			//to do when pressed
		}
		lastButtonPress = millis();
	}

  //------button 3------
	int btnState3 = digitalRead(BT3);
	if (btnState3 == LOW) {
		if (millis() - lastButtonPress > 50) {
			//to do when pressed
		}
		lastButtonPress = millis();
	}
	delay(1);
}
