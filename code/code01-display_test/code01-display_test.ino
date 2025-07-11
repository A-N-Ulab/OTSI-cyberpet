#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


int i = 0;

SSD1306Wire  factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

const int SCLpin = 42;
const int SDApin = 41;



void setup()
{
    Wire.begin(SDApin, SCLpin);
  Serial.begin(9600);
  while (!Serial);             
  Serial.println("\nI2C Scanner");
}

void loop()
{
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
         Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
     }
     else if (error==4)
     {
      Serial.print("Unknown error at address 0x");
      if (address<16)
         Serial.print("0");
      Serial.println(address,HEX);
     }    
    }
    if (nDevices == 0)
       Serial.println("No I2C devices found\n");
    else
       Serial.println("done\n");
    delay(5000);           // wait 5 seconds for next scan
}