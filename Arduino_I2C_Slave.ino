/* This provides some easy access to the Arduino peripherals and Libraries via an 8 byte transfer
   run any sketch and include this snippet to provide readings to the ESP8266 Via I2C using the array.
   ESP8266 connected to I2C via pins 4(SDA) and 5(SCL), the SHT21 is directly connected to ESP8266 pins
   Arduino pins 4 and 5 are connected via 5v to 3v converter to the ESP8266 pins 4 and 5.
   The 8 bytes are arranged - bytes 0+1 are a 2 char ID, bytes 2-5 Arduino A0 to A3 reading
   bytes 6 Humidity and 7 Temperature, presenting data as int allows very simple transfer.
   ESP8266 pin0 Used to Reset this Arduino and ESP8266 pin2 to send Request for data (RTS) to this Arduino.....
*/
#include <Wire.h>
#include "HTU21D.h"

HTU21D myHTU21D;

// Send variables
int Hint,Tint;
char msg[8];

// Recieved variables
String ArdData;

// Measurement timer
unsigned long ulNextMeas_ms=1000;
int RTSState;                        // Current RTS State
int lastRTSState = HIGH;             // Previous RTS State
unsigned long lastResponseTime = 0;  // Last RTS Toggled time
unsigned long ResponseDelay = 50;    // Delay to provide time for pin to settle.....

void ReadSensors() {
  Wire.write(msg);             // Response to Master Request
}


void setup() {
  pinMode(2, INPUT); 
 // Serial.begin(115200);
 // Serial.println("\r\n\r\n");
  Wire.begin(8);               // join i2c bus with address #8
  Wire.onRequest(ReadSensors); // register event
  delay(100);
  myHTU21D.begin();

}

void loop() {
  delay(10);
  int RTSFlag = digitalRead(2);
  if (RTSFlag != lastRTSState) {
    lastResponseTime = millis();
  }
  if ((millis() - lastResponseTime) > ResponseDelay) {
    if (RTSFlag != RTSState) {
      RTSState = RTSFlag;
      if (RTSState == LOW) {
  msg[0]='A';  //  Used to identify which Arduino, Use as many as needed adjust the Address!
  msg[1]='1';  //  These 2 bytes are used to help verify the data in the Master Sketch
  String TempRp="";
  TempRp+=msg[0];
  TempRp+=msg[1];
  for(int i=0;i<4;i++){
  int sensorValue = analogRead(i);
  sensorValue=constrain(map(sensorValue, 0, 1020, 0, 254), 0, 254);
  msg[i+2]=sensorValue; 
  TempRp+=sensorValue;
  }
  Hint=myHTU21D.readCompensatedHumidity();
  Tint=myHTU21D.readTemperature();
  msg[6]=Hint;
  msg[7]=Tint;
  TempRp+=Hint;
  TempRp+=Tint;
  //Serial.println(TempRp);
      }
    }
  }
    lastRTSState = RTSFlag;    
}
