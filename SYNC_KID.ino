#include "SD.h" // import SD card libray
#include "TMRpcm.h"
#include "SPI.h" // import SD card SPI libray
#include <LedControl.h>  // import LED matrix libray
TMRpcm tmrpcm;

// Define the pins for the joystick
const int xPin = A2;
const int yPin = A3;

#define SD_card 10 //SD card pin
int CLK = 5; // LED MATRIX pins on 5,6,7
int CS = 6;
int DIN = 7; 
LedControl lc = LedControl(DIN,CLK,CS,1); //Create instance of LedControl class

//Define LED UI
byte HEART[8]= {0x00,0x66,0xff,0xff,0x7e,0x3c,0x18,0x00};
byte HAPPY[8]= {0x3c,0x42,0xa5,0x81,0xa5,0x99,0x42,0x3c};
byte MAD[8]= {0x3c,0x42,0xe7,0x81,0xbd,0x81,0x42,0x3c};

//Define LED function
void printByte(byte character[]) {
  int i = 0;
  for(i=0;i<8;i++) {
    lc.setRow(0,i,character[i]);
  }
}


unsigned long previousMillisJoystick = 0;  // last time joystick was read
const long intervalJoystick = 1000;  // interval to read joystick (milliseconds)
unsigned long previousMillis = 0;  // last time an action was performed
const long interval = 2500;  // interval length 2.5 seconds


void setup() {
  lc.shutdown(0,false); //turn on LED matrix
  lc.setIntensity(0,5); //set LED matrix brightness
  lc.clearDisplay(0); //turns off LED matrix to clear display

  tmrpcm.speakerPin=9;
  tmrpcm.setVolume(6);
  Serial.begin(9600);

  if(!SD.begin(SD_card)){
  Serial.println("SD fail");
  return;
  }
}
// Read the joystick values
int xValue;
int yValue;

void loop() {
unsigned long currentMillis = millis();
xValue = analogRead(xPin);
yValue = analogRead(yPin);
if (currentMillis - previousMillisJoystick >= intervalJoystick) {
    previousMillisJoystick = currentMillis;
// Print the joystick values to the Serial Monitor
Serial.println(); //map other quadrants 70-100
Serial.print("X: ");
Serial.print(xValue);
Serial.print(" | Y: ");
Serial.print(yValue);
Serial.println();

// Determine the color based on the joystick position
  if (xValue > 600) { // if joystick toggle LEFT: GREEN (Ready to call you)
  Serial.write('4'); //send signal 4 to parent device
} else if (xValue < 400) { // if joystick toggle RIGHT: GREEN (Ready to call you)
  Serial.write('5'); //send signal 5 to parent device
} else if (yValue < 400) { // if joystick toggle UP (I’m sad, help)
  Serial.write('6'); //send signal 6 to parent device
} else if (yValue > 600) { // if joystick toggle DOWN (I’m busy, dont bother)
  Serial.write('7'); //send signal 7 to parent device
} else { 
  // otherwise do nothing
}
}

  if(Serial.available()) { //BLUETOOTH RECEIVER
    char val = Serial.read();
    //execute happy emoji + NAG2.wav if signal 1 is received
    //and current time is more than 2.5 seconds 
    if(val == '1' && currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      printByte(HAPPY); // user 2 is happy
      tmrpcm.play("NAG2.wav"); // i'm okay audio
    }
    //execute happy emoji + NAG.wav if signal 1 is received
    //and current time is more than 2.5 seconds 
    else if(val == '2' && currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      printByte(HEART);  // user 2 is ok
      tmrpcm.play("NAG.wav"); // somewhat miss you audio
    }
    //execute happy emoji + NAG3.wav if signal 1 is received
    //and current time is more than 2.5 seconds 
    else if(val == '3' && currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      printByte(MAD); // user 2 is mad
      tmrpcm.play("NAG3.wav"); // extremely miss you audio
    }
  }
  }