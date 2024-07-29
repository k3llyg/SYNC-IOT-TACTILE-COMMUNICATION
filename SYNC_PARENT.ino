#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

//Define dial + value
const int dialPin = A0;
const int ok = 4;
const int miss = 1016;

// Define the pins for the RGB LED
const int redPin = 9;
const int greenPin = 11;
const int bluePin = 10;


unsigned long previousMillis = 0;
unsigned long previousMillisDial = 0; 
const long interval = 100;
const long intervalDial = 200;

int count = 0;


void setup() {
  pinMode(dialPin, INPUT);

  // Set the RGB LED pins as output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  myservo.attach(3);

  Serial.begin(9600);
}

int dialPercentage() {
  int dialVal = analogRead(dialPin);
  int dialLevel = map(dialVal, ok, miss, 0, 100);

  Serial.print(dialLevel);
  Serial.println("%");
  delay(1000);
  return dialLevel;
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, 255 - red);
  analogWrite(greenPin, 255 - green);
  analogWrite(bluePin, 255 - blue);
}

void servomotion(){
myservo.write(0);
delay(100);
myservo.write(180);
delay(100);
myservo.write(0);
delay(100);
myservo.write(180);
delay(100);
}


void loop() {
  unsigned long currentMillis = millis();

  if(Serial.available() && currentMillis - previousMillis >= interval) { //BLUETOOTH RECEIVER
    previousMillis = currentMillis;
    char val = Serial.read();
    //Serial.print("Received: ");
    //Serial.println(val);
    if(val == '4'){ //if signal 4 received
      // JOYSTICK LEFT: Green  (Ready to call you)
      setColor(0, 255, 0);
      servomotion(); // servo moves
    }

    else if(val == '4'){
      // JOYSTICK LEFT: Green  (Ready to call you)
      setColor(0, 255, 0);
      servomotion(); // servo moves
    }
    
    else if(val == '5') {
      // JOYSTICK RIGHT: Green (Ready to call you)
      setColor(0, 255, 0);
      servomotion(); // servo moves
    }

    else if(val == '6'){
      // JOYSTICK UP: Blue (I'm sad, help)
      setColor(0, 0, 255);
      servomotion(); // servo moves
    }

    else if(val == '7'){
      // JOYSTICK DOWN: Purple (I'm busy, dont bother)
      setColor(255, 0, 255);
      servomotion(); // servo moves
    }

    else{ 
      //otherwise do nothing
    }
  }

//current time is more than 0.2 seconds 
if (currentMillis - previousMillisDial >= intervalDial) {
    previousMillisDial = currentMillis;
  int dialLevel = dialPercentage(); //read dial level
  //if dial precentage more than 70%
  if (dialLevel >= 70) {
    //send signal '1' to child device
    Serial.write('1');
  }

//if dial precentage between 30-70%
  else if (dialLevel > 30 && dialLevel < 70) {
    //send signal '2' to child device
    Serial.write('2');
  }

//if dial precentage less than 30%
  else if (dialLevel <= 30) {
    //send signal '3' to child device
    Serial.write('3');
  }
}
}