// ALL Connections to Arduino are via a 4 way 5v to 3v convertor........Maybe use a 3v Arduino....???
// Very Simple access to 4 * 8 bit adc's and the Arduino Libraries.....
// Adjust the read mapping and shift the data into a 32bit transfer for full access.....
// This is a simple application of I2C to add access to any arduino data
// that you can squeeze between reads, easily adjustable....
// Using Pin0 (Fash button) to HOLD Arduino in reset while flash booting ESP by connecting
// the 5v side of the convertor directly to Arduino Reset.
// To upload an Arduino Sketch to the Arduino remove link from reset to pin0....
// ESP is powered from 3v3 LDO - 5v from a CP2102 or the Arduino 5v USB Only 1 Attached !!!
// Input of LDO and USB 5v connected..........

// Using a write to Pin2 to request Data Read on I2C to be checked by Arduino Slave.......
// This will allow an ESP8266-01 to run the I2C on Pins 1 & 3 and the pins for reset and read Flag....
// Simply do not use serial.......Use ESP Web Server to present data......


#include <Wire.h>
extern "C" 
{
#include "user_interface.h"
}


ADC_MODE(ADC_VCC);

byte four,three,two,one,Hum,Temp;

int CoUnt,FCoUnt,ReSet;

unsigned long LooPTimer=2000; // Set to fire at start...reloads with a value when first triggered

void setup() {
  Serial.begin(115200);   // start serial output
  Serial.println("\r\n\r\nStarting I2C Arduino Slave.......\r\n\r\n");
  digitalWrite(0, HIGH);
  pinMode(2, OUTPUT);     // I2C Request To Send, ESP8266 to Slave, 0 = Request Ready, 1 = idle
  digitalWrite(2, HIGH);
  delay(3000);            // allow all to settle before sending reset to Arduino.....
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);  // Using Fash button
  delay(10);
  digitalWrite(0, LOW);
  delay(5);               // 5 microsec pulse to reset Arduino.....
  digitalWrite(0, HIGH);
  delay(3000);            // Arduino Startup delay
  Wire.begin(4,5);        // start i2c bus
  Serial.println("Started I2C Arduino Slave....Requesting Data...\r\n\r\n");
  delay(1000);
}

void loop() {

if(FCoUnt>1){             // Reset Arduino after 2 errors from I2C......
  FCoUnt=0;
  ReSet++;                // Reset ESP after 10 Errors......
  if(ReSet==5){ESP.restart();}
  Serial.println("\r\nResetting I2C Arduino Slave.......\r\n\r\n");
  delay(10);
  digitalWrite(0, LOW);
  delay(5);               // 5 microsec pulse to reset Arduino.....
  digitalWrite(0, HIGH);
  delay(3000);            // Arduino Startup delay
  Wire.requestFrom(8, 8);
  delay(100);
}
  unsigned long currentMillis = millis();
if (currentMillis>=LooPTimer) 
 {
  digitalWrite(2, LOW);      // Set Request To Send Active....
  delay(130);                // Set this for the response time of Your Arduino Loop......
  LooPTimer = currentMillis+1000; // Reload the timer with an ESP loop time of 5 Seconds.......
  String New1="";
   CoUnt++;
 //  if(CoUnt==8 || CoUnt==41){delay(500);}
  Wire.requestFrom(8, 8);    // request 8 bytes from slave device #8
int lOop=0;
  while (Wire.available()) { // slave may send less than requested
if(lOop==2){
    one = Wire.read();
    two = Wire.read();;
    three = Wire.read();
    four = Wire.read();
    Hum = Wire.read();
    Temp = Wire.read();
    }
  lOop++;
  char c = Wire.read(); // receive a byte as character 
  New1+=c;
  }
New1=New1.substring(0,2);// Simple error checking.........
if(New1=="A1" && one!=255 && four!=255){
  float pfVcC =  ESP.getVcc()-240;  //  Use a multimeter to find simple adjustment values.......
Serial.println("|_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_¬");
Serial.print("\r\n  Device ID Value : ");
Serial.println(New1);
Serial.print("\r\n1 Arduino A0 Value  ");
Serial.print((float(one*2)/100)-0.3);
Serial.print(" Volts\r\n2 Arduino A1 Value  ");
Serial.print((float(two*2)/100)-0.3);  
Serial.print(" Volts\r\n3 Arduino A2 Value  ");
Serial.print((float(three*2)/100)-0.3);
Serial.print(" Volts\r\n4 Arduino A3 Value  ");
Serial.print((float(four*2)/100)-0.3);
Serial.print(" Volts\r\n5     ESPVcc Value  ");
Serial.print(pfVcC/1000, 2);
Serial.print(" Volts\r\n6    SHT21 Humidity ");
Serial.print(Hum);
Serial.print("\r\n7 SHT21 Temperature ");
Serial.print(Temp);
Serial.print("\r\n\r\nLoops ");
Serial.print(CoUnt);
Serial.print("\r\nSingle Errors ");
Serial.print(FCoUnt);
Serial.print("\r\nArduino Resets(+2 errors) ");
Serial.println(ReSet);
}else{
    FCoUnt++;
    Serial.println("\r\nError in IIC Retrying.... ");
    Serial.print("\r\nLoops ");
    Serial.println(CoUnt);
    Serial.print("\r\nSingle Errors ");
    Serial.println(FCoUnt);
    Serial.print("\r\nArduino Resets(+2 errors) ");
    Serial.println(ReSet);
    }
 digitalWrite(2, HIGH);      // Set Request Idle
 delay(150);
 }
yield();
}
