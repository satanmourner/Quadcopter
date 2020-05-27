#include <Servo.h>
#include <SoftwareSerial.h>

//**************************************************************
#define PIN_AILERONS 8
#define PIN_ELEVATOR 9
#define PIN_THROTTLE 10
#define PIN_RUDDER 11
#define PIN_GEAR 12

//for output u should use servo and attach these pins
Servo ELEVATOR_TO_FC; 
Servo AILERON_TO_FC;
Servo THROTTLE_TO_FC;
Servo RUDDER_TO_FC;

int throttle, aileron;

//***************************************************************
int RXPinS = 3;
int TXPinS = 4;

SoftwareSerial mySerial(RXPinS, TXPinS);


void setup() {

  Serial.begin(9600);
  /*************************************************** main *******************************************************/
  pinMode(PIN_ELEVATOR, INPUT);
  pinMode(PIN_AILERONS, INPUT);
  pinMode(PIN_THROTTLE, INPUT);
  pinMode(PIN_RUDDER, INPUT);
  pinMode(PIN_GEAR, INPUT);

  ELEVATOR_TO_FC.attach(A3, 1000, 2000);
  AILERON_TO_FC.attach(A1, 1000, 2000);
  THROTTLE_TO_FC.attach(A0, 1000, 2000);
  RUDDER_TO_FC.attach(A2, 1000, 2000);

  mySerial.begin(9600);
  /*************************************************** SMS *******************************************************/
  mySerial.println("******SIMCARD DATA******");
  mySerial.println("AT+CMGF=1");//text mode
  mySerial.println("AT+CNMI=1,2,0,0,0");//read sms
}


//returns 1 if its on Full-Automated
int checkAutoTrigger() {

  const int MIN_PULSE = 1099;

  int pl = pulseIn(PIN_GEAR, HIGH);

  int result = !((pl - MIN_PULSE) <= 0);

//  const int mode = result ? OUTPUT : INPUT;
//
//  pinMode(PIN_ELEVATOR, mode);
//  pinMode(PIN_AILERONS, mode);
//  pinMode(PIN_THROTTLE, mode);
//  pinMode(PIN_RUDDER, mode);

  Serial.print("Automations >>>>>>>>>>> : ");
  Serial.print(result);
  Serial.print("\n");  

  return result;
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

bool automated = false;
void loop() {
  
  const int autoTrigger = checkAutoTrigger();

  int elevator_pulse = pulseIn(PIN_ELEVATOR, HIGH,40000);
  int aileron_pulse = pulseIn(PIN_AILERONS, HIGH,40000);
  int throttle_pulse = pulseIn(PIN_THROTTLE, HIGH,40000);
  int rudder_pulse = pulseIn(PIN_RUDDER, HIGH,40000);
  int gear_pulse = pulseIn(PIN_GEAR, HIGH,40000);

  ELEVATOR_TO_FC.write(elevator_pulse);
  AILERON_TO_FC.write(aileron_pulse);
  THROTTLE_TO_FC.write(throttle_pulse);
  RUDDER_TO_FC.write(rudder_pulse);
 
//  if (autoTrigger && !automated) {
//
//
//    ELEVATOR_TO_FC.writeMicroseconds(1115);
//    AILERON_TO_FC.writeMicroseconds(1051);
//    THROTTLE_TO_FC.writeMicroseconds(1095);
//    RUDDER_TO_FC.writeMicroseconds(1842);
//
//    Serial.print("the model is armed\n");
//
//    delay(1000);
//
//    automated = true;
//  }
//
//  if(automated){
//
//    ELEVATOR_TO_FC.writeMicroseconds(elevator_pulse);
//    AILERON_TO_FC.writeMicroseconds(aileron_pulse);
//    THROTTLE_TO_FC.writeMicroseconds(1834);
//    RUDDER_TO_FC.writeMicroseconds(rudder_pulse); 
//  }

//  Serial.print("Pulse In elevator_pulse : \t");
//  Serial.print(elevator_pulse);
//  Serial.print("\n");
//  
//
//  Serial.print("Pulse In aileron_pulse : \t");
//  Serial.print(aileron_pulse);
//  Serial.print("\n");
//
//  Serial.print("Pulse In throttle_pulse : \t");
//  Serial.print(throttle_pulse);
//  Serial.print("\n");
//
//  Serial.print("Pulse In rudder_pulse : \t");
//  Serial.print(rudder_pulse);
//  Serial.print("\n");
//
//  Serial.print("Pulse In gear_pulse : \t\t");
//  Serial.print(gear_pulse);
//  Serial.print("\n");
//
//  Serial.print("\n\n");
//  delay(1000);

  if(autoTrigger) {
    String message = SMSRead();
    textMessage(message);
    CallKalak();
  }
}

void textMessage(String message) {
   if(message.startsWith("\r\n+CMT: ")) {
    Serial.println(message.substring(9,22)); //number send sms
    
     int len = message.length(); //length of message 
     
     Serial.println(message.substring(51, len-2)); //51 start message to end exept \r\n (-2)=
  }
}

void CallKalak() {
   THROTTLE_TO_FC.write(throttle);
   delay(3000);
   AILERON_TO_FC.write(aileron);
   delay(100000);

   for(int throRaduce = 0; throRaduce < 2000; throRaduce+=50) {
    THROTTLE_TO_FC.write(throttle - thruRaduce);
    delay(100);
   }
}
