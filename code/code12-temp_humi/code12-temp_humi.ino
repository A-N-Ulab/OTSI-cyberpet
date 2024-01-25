#include <dht11.h>
#define DHT11PIN 6

dht11 DHT11;

float temp, humid;

void  setup()
{
  Serial.begin(9600);
}

void loop(){
  int chk = DHT11.read(DHT11PIN);
  temp = (float)DHT11.temperature;
  humid = (float)DHT11.humidity;
  
  Serial.println(temp);
  Serial.println(humid);

  delay(2000);

}