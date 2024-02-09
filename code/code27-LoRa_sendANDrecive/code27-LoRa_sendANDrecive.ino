#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>

SX1262 radio = new Module(8, 14, 12, 13); //nss, dio1, nrst, busy

//lora
String dataSend = "OTSI", dataRecive;
int stateRecive;
long tc2, delaySendLora;
bool conYorN = false;

void setup() {
  Serial.begin(9600);

  pinMode(0, INPUT_PULLUP);

  SPI.begin(9, 11, 10, 8); //sck, miso, mosi, nss
  if (radio.begin() != RADIOLIB_ERR_NONE) Serial.print("Failed to connect");
}

void loop() {
  tc2 = millis();
  if(tc2 - delaySendLora >= 5000){
    radio.transmit(dataSend);
    delaySendLora = tc2;
  }

  stateRecive = radio.receive(dataRecive);

  Serial.println(dataRecive);
  if(dataRecive == "OTSI"){
    dataSend = "connected";
  }
  else if(dataRecive == "connected"){
    conYorN = true;
  }


  if(conYorN && dataRecive == "connected"){
    communicate();
  }
}

void communicate(){
  Serial.print(digitalRead(0));
  Serial.println("        CONNECTED");
}