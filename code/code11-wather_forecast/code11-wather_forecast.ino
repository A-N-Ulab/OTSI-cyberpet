#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
const char* ssid = "UPC0520860";
const char* password = "Ae8kdjbmaerx";
String openWeatherMapApiKey = "a9256737faca307d52c2fa7e1aeed939";
String city = "Katowice";
String countryCode = "PL";
String WthRef;
String jsonBuffer;
unsigned long lastTime = 0;
unsigned long timerDelay = 10000, t_WhT;



void setup() {
  Serial.begin(9600);
  
}

void loop(){
  WeatherForecast();
}

void WeatherForecast(){
  //------connect to WiFi------
  WiFi.begin(ssid, password);
  //------check the connection------
  while (WiFi.status() != WL_CONNECTED && millis()-t_WhT < 5000) delay(500);
  if(WiFi.status() != WL_CONNECTED && millis()-t_WhT > 5000) WthRef = "Updated";
  else WthRef = "Updated";
  //------init and get the time------


  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  String payload = "{}"; 
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
