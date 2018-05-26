#include <SoftwareSerial.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 uv = Adafruit_VEML6070();
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

int groundMoisturePin = A0;

const int TX_BT = 6;
const int RX_BT = 7 ;
SoftwareSerial btSerial(TX_BT, RX_BT);

#define DHTPIN 2     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

void printGroundMoisture(){
  // 0-300 dry soil , 300-700 humid soil, 700-950 in water
  int groundMoistureValue = analogRead(groundMoisturePin);
  Serial.print("Ground Moisture: "); Serial.println(groundMoistureValue);
  btSerial.print("Ground Moisture: "); btSerial.println(groundMoistureValue);
}

void printUVLevel(){
  Serial.print("UV light level: "); Serial.println(getUVLevel(uv.readUV())); 
  btSerial.print("UV light level: "); btSerial.println(getUVLevel(uv.readUV())); 
}

int getUVLevel(int value){
  if(value <= 2240) return 0;
  if(value <= 4482) return 1;
  if(value <= 5976) return 2;
  if(value <= 8216) return 3;
  return 4;
}

void printLightLevel(){
  sensors_event_t event;
  tsl.getEvent(&event);
  if (event.light) { 
    Serial.print("Light level: "); Serial.print(event.light); Serial.println(" lux");
    btSerial.print("Light level: "); btSerial.print(event.light); btSerial.println(" lux");
   }
  else Serial.println("Sensor overload"); btSerial.println("Sensor overload");  
}

void printTempHumidityLevel(){
  int chk;
  float hum; 
  float temp;
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");
  btSerial.print("Temp: "); btSerial.print(temp); btSerial.println(" Celsius");
  Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");
  btSerial.print("Temp: "); btSerial.print(temp); btSerial.println(" Celsius");
}

void setup(){
  pinMode(groundMoisturePin, INPUT);
  
  Serial.begin(9600);

  btSerial.begin(9600);

  // Temp & Humidity Sensor
  dht.begin();

  // UV Sensor
  uv.begin(VEML6070_4_T); 

  // Light Sensor
  tsl.enableAutoRange(true); 
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
}

void loop(){
  printGroundMoisture();
  printLightLevel();
  printUVLevel();
  printTempHumidityLevel();
  Serial.println("");
  btSerial.println("");
  delay(2000);  
}
