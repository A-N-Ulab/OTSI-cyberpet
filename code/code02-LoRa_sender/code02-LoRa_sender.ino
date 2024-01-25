/*
  Code Base from RadioLib: https://github.com/jgromes/RadioLib/tree/master/examples/SX126x

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>

#define LoRa_MOSI 10
#define LoRa_MISO 11
#define LoRa_SCK 9

#define LoRa_nss 8
#define LoRa_dio1 14
#define LoRa_nrst 12
#define LoRa_busy 13

SX1262 radio = new Module(LoRa_nss, LoRa_dio1, LoRa_nrst, LoRa_busy);

void setup(){
  Serial.begin(9600);
  SPI.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_nss);

  // initialize SX1262 with default settings
  if (radio.begin() != RADIOLIB_ERR_NONE){
    Serial.print("Failed to connect");
  }
}

void loop(){
  int state = radio.transmit("Hello World!");

  if(state == RADIOLIB_ERR_TX_TIMEOUT) Serial.println("timeout");
  
  delay(1000);
}