#include <SoftwareSerial.h>
#include <OneWire.h>
//#include <string>

OneWire ds(10);
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28

const int TX_BT = 6;
const int RX_BT = 7 ;

SoftwareSerial btSerial(TX_BT, RX_BT);

void setup() {
  Serial.begin(9600);
  Serial.println("USB Connected");
  btSerial.begin(9600);
}
 

void loop() {
	
//  if(btSerial.available()) {
//      char bt = btSerial.read();
//	    Serial.write(bt);
//      if(bt == 'l') //lamp
//	  {
//		  
//			  
//	  }
//
//	  else if(bt == 'f') //fan
//	  { 
//			  
//	  }
//  }
  
  btSerial.print("Test");
  
}
