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

#define DHTPIN 3     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

int getGroundMoisture(){
  // 0-300 dry soil , 300-700 humid soil, 700-950 in water
  
  int groundMoistureValue = analogRead(groundMoisturePin);
  
  Serial.print("Ground Moisture: "); Serial.println(groundMoistureValue);
  
  return groundMoistureValue;  
}

int getUVLevel(){
  Serial.print("UV light level: "); Serial.println(getUVLevel(uv.readUV())); 

  return getUVLevel(uv.readUV());
}

int getUVLevel(int value){
  if(value <= 2240) return 0;
  if(value <= 4482) return 1;
  if(value <= 5976) return 2;
  if(value <= 8216) return 3;
  return 4;
}

float getLightLevel(){
  sensors_event_t event;
  tsl.getEvent(&event);
  
  Serial.print("Light level: "); Serial.print(event.light); Serial.println(" lux");
  
  return event.light;  
}

float getTemperature(){  
  float temp = dht.readTemperature();
  
  Serial.print("Temp: "); Serial.print(temp); Serial.println(" Celsius");

  return temp;
}

float getHumidity(){  
  float hum = dht.readHumidity();
  
  Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");

  return hum;
}

String getPlantSample(){
  int groundMoisture = getGroundMoisture();
  float light = getLightLevel();
  int uv = getUVLevel();
  float temp = getTemperature();
  float humidity = getHumidity();

  String plantSample;
  plantSample.concat("#");
  plantSample.concat("groundmoisture|");
  plantSample.concat(groundMoisture); 
  plantSample.concat("#");
  plantSample.concat("light|");
  plantSample.concat(light);
  plantSample.concat("#");
  plantSample.concat("uv|");
  plantSample.concat(uv);
  plantSample.concat("#");
  plantSample.concat("temp|");
  plantSample.concat(temp);
  plantSample.concat("#");
  plantSample.concat("humidity|");
  plantSample.concat(humidity);
  return plantSample;
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
  Serial.println(getPlantSample());
  btSerial.println(getPlantSample());
  delay(2000);  
}
