#include <SoftwareSerial.h>

int RXPinS = 3;
int TXPinS = 4;

SoftwareSerial mySerial(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.println("******SIMCARD DATA******");
  mySerial.println("AT+CMGF=1");//text mode
  //SerialInfo();
  mySerial.println("AT+CNMI=1,2,0,0,0");//read sms
  //SerialInfo();
}

String SMSRead() {
  
  String tempStr;

  while(mySerial.available()) {
    char tempCh = mySerial.read();
    tempStr.concat(tempCh);
    delay(10);
  }
  return tempStr;
}

void loop() {

  String message = SMSRead();
//  
//  if(mySerial.available() > 0) 
//    Serial.println(mySerial.read());
    
  if(message.startsWith("\r\n+CMT: ")) {
    Serial.println(message.substring(9,22)); //number send sms
    
     int len = message.length(); //length of message 
     
     Serial.println(message.substring(51, len-2)); //51 start message to end exept \r\n (-2)
//     
//     if(message = "on" or "ON" or "On" or "oN") {
//      Serial.println("turn on Quad");
//     }
  }
}

//void SerialInfo() {
//for command on serial monitor 
//  while(Serial.available()) {
//    mySerial.write(Serial.read());
//  }
//  while(mySerial.available()) {
//    Serial.write(mySerial.read());
//  }
//} 
