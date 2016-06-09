/*
 Fading

 This example shows how to fade an LED using the analogWrite() function.

 The circuit:
 * LED attached from digital pin 9 to ground.

 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe

 http://www.arduino.cc/en/Tutorial/Fading

 This example code is in the public domain.

 */


int ledPin = 9;    // LED connected to digital pin 9

void setup() {
  // nothing happens in setup
}

void loop() {
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}

/* ==================================================================================================================================================
         Project: MT8870 DTMF Servo sketch
          Author: Scott C
         Created: 4th August 2015
     Arduino IDE: 1.6.4
         Website: http://arduinobasics.blogspot.com/p/arduino-basics-projects-page.html
     Description: This project will allow you to control a Servo motor using an Arduino UNO and a MT8870 DTMF Module.
                  The DTMF signal is received through the 3.5mm port of the DTMF module and is decoded. We will use the
                  decoded output to control the position of the Servo. A SG-5010 Servo motor was used in this project.
===================================================================================================================================================== */
//This sketch uses the Servo library that comes with the Arduino IDE
#include <Servo.h>


//Global variables-----------------------------------------------------------------------------------------
Servo SG5010;             // The SG5010 variable provides Servo functionality
int servoPosition = 0;    // The servoPosition variable will be used to set the position of the servo
byte DTMFread;            // The DTMFread variable will be used to interpret the output of the DTMF module.
const int STQ = 3;        // Attach DTMF Module STQ Pin to Arduino Digital Pin 3
 const int Q4 = 4;        // Attach DTMF Module Q4  Pin to Arduino Digital Pin 4
 const int Q3 = 5;        // Attach DTMF Module Q3  Pin to Arduino Digital Pin 5
 const int Q2 = 6;        // Attach DTMF Module Q2  Pin to Arduino Digital Pin 6
 const int Q1 = 7;        // Attach DTMF Module Q1  Pin to Arduino Digital Pin 7


/*=========================================================================================================
setup() : will setup the Servo, and prepare the Arduino to receive the MT8700 DTMF module's output.
========================================================================================================== */
void setup() {
  SG5010.attach(9);             // The Servo signal cable will be attached to Arduino Digital Pin 9
  SG5010.write(servoPosition);  // Set the servo position to zero.

  //Setup the INPUT pins on the Arduino
  pinMode(STQ, INPUT);
  pinMode(Q4, INPUT);
  pinMode(Q3, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q1, INPUT);
}

/*=========================================================================================================
loop() : Arduino will interpret the DTMF module output and position the Servo accordingly
========================================================================================================== */
void loop() {
  if(digitalRead(STQ)==HIGH){       //When a DTMF tone is detected, STQ will read HIGH for the duration of the tone.
    DTMFread=0;
    if(digitalRead(Q1)==HIGH){      //If Q1 reads HIGH, then add 1 to the DTMFread variable
      DTMFread=DTMFread+1;
    }
    if(digitalRead(Q2)==HIGH){      //If Q2 reads HIGH, then add 2 to the DTMFread variable
      DTMFread=DTMFread+2;
    }
    if(digitalRead(Q3)==HIGH){      //If Q3 reads HIGH, then add 4 to the DTMFread variable
      DTMFread=DTMFread+4;
    }
    if(digitalRead(Q4)==HIGH){      //If Q4 reads HIGH, then add 8 to the DTMFread variable
      DTMFread=DTMFread+8;
    }
    servoPosition = DTMFread * 8.5;   //Set the servoPosition varaible to the combined total of all the Q1 to Q4 readings. Multiply by 8.5 to amplify the servo rotation.
  }
  SG5010.write(servoPosition);        //Set the servo's position according to the "servoPosition" variable.
}

//http://arduinobasics.blogspot.cl/2015/08/mt8870-dtmf-dual-tone-multi-frequency.html
