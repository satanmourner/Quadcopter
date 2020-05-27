//Gy271 ------> compass
//comminucate with arduino by i2c(serial comminuucation)
#include <Wire.h>

int16_t ComX,ComY,ComZ;//3-axis
//int16_t ComX_off,ComY_off,ComX_off;
double angle;//in degree
//int i = 0;

void setup() {
  Wire.begin(); //comminucate with i2c
  Wire.beginTransmission(0x0D); //start comminucate with gy271 -> adsress is 0x0D
  Wire.write(0x09); //register for config
  Wire.write(0x1D); //register for writing and continue on config
  Wire.endTransmission(true); //end comminucation
  
  Serial.begin(9600);

}

void loop() { //instead of loop 
  Wire.beginTransmission(0x0D);
  Wire.write(0x00); 
  Wire.endTransmission(false); // dont end
  Wire.requestFrom(0x0D, 6, true); //until get 6 output and then end

  ComX = Wire.read() <<8 | Wire.read();
  ComY = Wire.read() <<8 | Wire.read();
  ComZ = Wire.read() <<8 | Wire.read();

  
  angle= atan2((double)ComY,(double)ComX) * (180 / 3.14159265) + 180; // angle in degrees

//  Serial.print(" X : ");Serial.print(ComX);
//  Serial.print(" |Y : ");Serial.print(ComY);
//  Serial.print(" |Z : ");Serial.print(ComZ);

    //Print the approximate direction
 
    Serial.print("You are heading ");
    if((angle < 22.5) || (angle > 337.5 ))
        Serial.print("South");
    if((angle > 22.5) && (angle < 67.5 ))
        Serial.print("South-West");
    if((angle > 67.5) && (angle < 112.5 ))
        Serial.print("West");
    if((angle > 112.5) && (angle < 157.5 ))
        Serial.print("North-West");
    if((angle > 157.5) && (angle < 202.5 ))
        Serial.print("North");
    if((angle > 202.5) && (angle < 247.5 ))
        Serial.print("NorthEast");
    if((angle > 247.5) && (angle < 292.5 ))
        Serial.print("East");
    if((angle > 292.5) && (angle < 337.5 ))
        Serial.print("SouthEast");
 
    Serial.print(": Angle between X-axis and the South direction ");
    if((0 < angle) && (angle < 180) )
    {
        angle=angle;
    }
    else
    {
        angle=360-angle;
    }
    Serial.print(angle,2);
    Serial.println(" Deg");
    delay(300);
}
