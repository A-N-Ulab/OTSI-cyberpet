int main_utc = 1;
const char* ssid = "UPC0520860";
const char* password = "Ae8kdjbmaerx";

//------libraries------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"
#include <virtuabotixRTC.h>
#include <Time.h>
#include "time.h"
#include "WiFi.h"
#include <dht11.h>


//------Displays init------
Adafruit_SSD1306 display1(128, 64, &Wire, 21, 500000UL);
Adafruit_SSD1306 display2(128, 64, &Wire1, -1, 400000UL);

//WiFi scanner
String WiFi_scan_encryption = "", WiFi_scan_SSID = "", tooLong;
int WiFi_scan_len, WiFi_enc_len, max_len;
bool first = true;
long t_s1, t_s2;

void setup() {
  //------DISPLAY'S SETUP-------
  //built-in display
  Wire.begin(17,18);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display1.clearDisplay();
  display1.display();
  //second display
  Wire1.begin(41,42);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display2.clearDisplay();
  display2.display();

  display2.setTextSize(1);
  display2.setTextColor(WHITE); 
  display2.setCursor(0, 0);

  Serial.begin(9600);
 

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
 
  t_s2 = millis();
}




void loop() {
  t_s1 = millis();
  display2.clearDisplay();
  display2.drawBitmap(0, 0, (const unsigned char *)frameBig, 128, 64, WHITE);
 
  if(t_s1-t_s2 > 10000 || first){
    first = false;
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n != 0){
      for (int i = 0; i < n; i++){
        switch (WiFi.encryptionType(i)){
          case WIFI_AUTH_OPEN:
            WiFi_scan_encryption = "open";
            break;
          case WIFI_AUTH_WEP:
            WiFi_scan_encryption = "WEP";
            break;
          case WIFI_AUTH_WPA_PSK:
            WiFi_scan_encryption = "WPA";
            break;
          case WIFI_AUTH_WPA2_PSK:
            WiFi_scan_encryption = "WPA2";
            break;
          case WIFI_AUTH_WPA_WPA2_PSK:
            WiFi_scan_encryption = "WPA+WPA2";
            break;
          case WIFI_AUTH_WPA2_ENTERPRISE:
            WiFi_scan_encryption = "WPA2-EAP";
            break;
          case WIFI_AUTH_WPA3_PSK:
            WiFi_scan_encryption = "WPA3";
            break;
          case WIFI_AUTH_WPA2_WPA3_PSK:
            WiFi_scan_encryption = "WPA2+WPA3";
            break;
          case WIFI_AUTH_WAPI_PSK:
            WiFi_scan_encryption = "WAPI";
            break;
          default:
            WiFi_scan_encryption = "unknown";
          }
        WiFi_scan_len = String(WiFi.SSID(i).c_str()).length();
        WiFi_enc_len = WiFi_scan_encryption.length();
        max_len = 15 - WiFi_enc_len;
        if(WiFi_scan_len > max_len) WiFi_scan_SSID = String(WiFi.SSID(i).c_str()).substring(0, max_len) + "... " + WiFi_scan_encryption;
        else WiFi_scan_SSID = String(WiFi.SSID(i).c_str()) + "... " + WiFi_scan_encryption;
        display2.setCursor(5, (10*(i+1))-5);
        display2.print(WiFi_scan_SSID);
        display2.display();
      }
    }
    t_s2 = t_s1;
  }
  WiFi.scanDelete();
}
