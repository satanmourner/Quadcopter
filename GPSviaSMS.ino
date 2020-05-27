#include <SoftwareSerial.h>
#include <TinyGPS++.h>

int RXPinS = 8;
int TXPinS = 9;
int RXPinG = 3;
int TXPinG = 4;

SoftwareSerial mySerial(RXPinS, TXPinS);
SoftwareSerial gpsSerial(RXPinG, TXPinG);

TinyGPSPlus gps;

bool turnOn = false;

struct Location {
  String lat;
  String lng;
  double latD;
  double lngD;
};

struct Location coorUser;

struct Location coorCurr;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  gpsSerial.begin(9600);
  mySerial.println("******SIMCARD DATA******");
  mySerial.println("AT+CMGF=1");//text mode
  mySerial.println("AT+CNMI=1,2,0,0,0");//read sms
  mySerial.listen();
}

String SMSRead() {
  
  String tempStr;
  //bcz we have 2 port of softwareSerial u should listen 

  while(mySerial.available()) {
    char tempCh = mySerial.read();
    tempStr.concat(tempCh);
  }
  return tempStr;  
}

void loop() {  //instead of loop

}

void serialEvent(){
  
  String message = SMSRead();
  String pureMessage = "";

  if(message!="")
    Serial.println(message);
  
  //if(mySerial.available() > 0) 
  //Serial.println(mySerial.read());
    
  if(message.startsWith("\r\n+CMT: ")) {
    Serial.println(message.substring(9,22)); //number send sms
    
     int len = message.length(); //length of message 

     pureMessage = message.substring(51, len-2);
     
     Serial.println(pureMessage); //51 start message to end exept \r\n (-2)
     
     if(pureMessage == "On") {
      Serial.println("turn on Quad");
      turnOn = true; //for arming and constant throttle
      getGPS();
     } else {
      Serial.println("where you want i go?");
      locationRead(pureMessage);
     }
  }
  
}

void getGPS() {
  
    gpsSerial.listen();
    
    while(gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());    
    }

    Serial.println((gps.satellites.value()));  //number of satellites
    //if(gps.location.isValid()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
  //      } else {
  //        Serial.println("Location: Not Available");
  //      }
    //}
    
    SendSMS();    
}

void SendSMS() {

  coorCurr.latD = gps.location.lat();
  coorCurr.lngD = gps.location.lng();
  
  mySerial.listen();
  
  //send lat,long to user
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+989371600514\"");
  delay(1000);
  mySerial.print("My cuurent coordinate");
  mySerial.print("Latitude: ");
  mySerial.print(coorCurr.latD, 9); //6 means more floating point if not put round to up `
  mySerial.print(",Longitude: ");
  mySerial.print(coorCurr.lngD, 9);
  mySerial.write(26);
  
  delay(1000);
}

void locationRead(String pureMessage) {

  int len = pureMessage.length();
  
  int beforeComma = pureMessage.indexOf(',');

  coorUser.lat = pureMessage.substring(0, beforeComma);
  coorUser.lng = pureMessage.substring(beforeComma+1, len);
  
  Serial.print("lattitude of destenation : ");Serial.println(coorUser.lat);
  Serial.print("longitude of destenation : ");Serial.println(coorUser.lng); 

  coorUser.latD = coorUser.lat.toDouble();
  coorUser.lngD = coorUser.lng.toDouble();
}
