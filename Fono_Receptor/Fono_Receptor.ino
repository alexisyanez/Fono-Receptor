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

/*
  DTMF.cpp - TEST code for DTMF library

  
DTMF  Arduino
Q1  12
Q2  11
Q3  10
Q4  9
STD 8
SPK2  3
SPK1  2

*/

#include "dtmf.h"

DTMF dtmf; // default set to DFRobot DTMF board pins, Speaker pins

// Setear contraseña
const int D1 = 2;
const int D2 = 2;
const int D3 = 1;
const int D4 = 2;

// Definir pin de Fade
#define FadePin 5  

void setup()
{
 Serial.begin(9600);
}

void loop()
{
int myDtmf;
if (Tonos()){
  if (Autenticacion()){
    FadeIn();
    while(true){
      myDtmf = dtmf.getDTMF();
      if(myDtmf == 9) FadeOut; break; // Detectar cuando se corta la llamada
      delay(80);
    }  
  }
}
    
}

  
  // Play a default number with all possibilities
  //dtmf.playDTMF();
  
  // Read DTMF codes one by one and print it on Serial
  /*int myDtmf;
  myDtmf = dtmf.getDTMF();
  if(myDtmf != -1) Serial.println(myDtmf);
  delay(80); // to avoid getting repeated output.*/


boolean Tonos(){ //
int rep = 0;
int outDTMF;
int m=0;

while(true){
outDTMF = dtmf.getDTMF();  
if(outDTMF == 0) ++rep; // Detectar Ring
if(outDTMF == -1) return false; break; // Detectar Silencio
if(rep == 5) return true; break; 
delay(80);
}
}

boolean Autenticacion(){
int exito=0;
int outDtmf;

outDtmf = dtmf.getDTMF();  
if(outDtmf == D1) ++exito;
delay(80);

outDtmf = dtmf.getDTMF();  
if(outDtmf == D2) ++exito;
delay(80);

outDtmf = dtmf.getDTMF();  
if(outDtmf == D3) ++exito;
delay(80);

outDtmf = dtmf.getDTMF();  
if(outDtmf == D4) ++exito;
delay(80);

if(exito == 4) return true;
else return false;
}

void FadeIn()
{
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(FadePin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}
void FadeOut()
{
  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(FadePin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}
