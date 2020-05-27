#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//rx,tx virtual with SoftwareSerial  library
int RXPin = 3;
int TXPin = 4;

//default baud of NEO-6M is 9600
int GPSBaud = 9600;

SoftwareSerial gpsSerial(RXPin, TXPin); //obj of SoftwareSerial
TinyGPSPlus gps; //obj of TinyGPSPlus

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  Serial.println("******GPS Data******");
}

void loop() {
  while (gpsSerial.available()){
//   Serial.write(gpsSerial.read()); ///---- raw data from gps
    gps.encode(gpsSerial.read()); //gps data encode
  }
  infoGPS();
  if(gps.charsProcessed() < 10) {
      Serial.println("no GPS detected");
      //while(true);
    }   
    delay(500);
}

void infoGPS() {
  
  Serial.println((gps.satellites.value()));  //number of satellites

  //if(gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
//  } else {
//  Serial.println("Location: Not Available");
//  }
  delay(1000);
}
