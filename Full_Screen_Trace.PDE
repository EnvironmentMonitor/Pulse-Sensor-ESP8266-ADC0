/*     PulseSensor Amped HRV Poncaire Plot

This is an HRV visualizer code for Pulse Sensor.
Use this with PulseSensorAmped_Arduino_1.1 Arduino code and the Pulse Sensor Amped hardware.
This code will draw a Poncaire Plot of the IBI (InterBeat Interval) passed from Arduino.
The Poncaire method of visualizing HRV trends is to plot the current IBI against the last IBI. 
key press commands included in this version:
  press 'S' or 's' to take a picture of the data window. (.jpg image)
  press 'c' to clear the graph 
Created by Joel Murphy, early 2013
This code released into the public domain without promises or caveats.
*/

import processing.serial.*;
PFont font, largerFont;
Serial port;

int test;                 // general debugger
int pulseRate = 0;        // used to hold pulse rate value from arduino (updated in serialEvent)
int Sensor = 0;           // used to hold raw sensor data from arduino (updated in serialEvent)
int IBI;                  // length of time between heartbeats in milliseconds (updated in serialEvent)
int ppgY;                 // used to print the pulse waveform
int[] PPG;                // array of live PPG datapoints
int[] beatTimeX;          // array of X coordinates
int[] beatTimeY;          // array of Y coordinates
int numPoints = 100;      // size of coordinate arrays. sets number of displayed datapoints
int upperH;

// initializing flags here
boolean pulse = false;    // made true in serialEvent when processing gets new IBI value from arduino


void setup() {
size(800,650); // Stage size
frameRate(60);
beatTimeX = new int[numPoints];    // these two arrays hold the Poncaire Plot data
beatTimeY = new int[numPoints];    // size of array determines number of displayed points
PPG = new int[800];                // PPG array that that prints heartbeat waveform
for (int i=0; i<800; i++){
 PPG[i] = height/2+65;             // initialize PPG widow with dataline at midpoint
}

//  LOAD THE FONTS
font = loadFont("Arial-BoldMT-36.vlw");
largerFont = loadFont("Arial-BoldMT-40.vlw");
textFont(font);
textAlign(CENTER);
rectMode(CENTER);

// FIND AND ESTABLISH CONTACT WITH THE SERIAL PORT
println(Serial.list());       // print a list of available serial ports
port = new Serial(this, Serial.list()[0], 115200); // choose the right baud rate
port.bufferUntil('\n');          // arduino will end each ascii number string with a carriage return 
port.clear();                    // flush the Serial buffer
}  // END OF SETUP
void draw(){
  background(0);
   noStroke();
text("Interval: "+IBI+"mS",width-105,20);
   if (IBI==0){
}else{
int pulseRate1=60000/IBI;
  text("Heart Rate: "+pulseRate1+"BPM",width-120,40);
}
// GRAPH THE LIVE SENSOR DATA
// move the y coordinate of the pulse waveform over one pixel left
 for (int i = 0; i < 800-1; i++){  
   PPG[i] = PPG[i+1];   // new data enters on the right at pulseY.length-1
 }
//   scale and constrain incoming Pulse Sensor value to fit inside the pulse window
  PPG[800-1] = int(map(ppgY,50,950,(height/2+65)+225,(height/2+65)-225));  
 fill(255,253,248);    // eggshell white
// rect(width-500,(height/2)+15,150,550);    // pulse window
 stroke(250,0,0);                         // use red for the pulse wave
 for (int i=1; i<800-1; i++){      // draw the waveform shape
   line(width-800+i,PPG[i],width-800+(i-1),PPG[i-1]);
 }
 noStroke();
 }  //END OF DRAW
 
