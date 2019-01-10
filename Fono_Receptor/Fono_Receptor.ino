/*
  Fading
 
 This example shows how to fade an LED using the analogWrite() function.
 
 The circuit:
 LED attached from digital pin 9 to ground.
 
 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe
 
 http://www.arduino.cc/en/Tutorial/Fading
 
 This example code is in the public domain.
 
 */
#include <DigiPotX9Cxxx.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(13, 12); //SIM800L Tx & Rx is connected to Arduino #13 & #12

DigiPot pot(8,9,10);
DigiPot pot2(2,9,10);

byte DTMFread; 
const int STQ = 3;        // Attach DTMF Module STQ Pin to Arduino Digital Pin 3
const int Q4 = 4;        // Attach DTMF Module Q4  Pin to Arduino Digital Pin 4
const int Q3 = 5;        // Attach DTMF Module Q3  Pin to Arduino Digital Pin 5
const int Q2 = 6;        // Attach DTMF Module Q2  Pin to Arduino Digital Pin 6
const int Q1 = 7;        // Attach DTMF Module Q1  Pin to Arduino Digital Pin 7

const int Ton = 11; 



/*=========================================================================================================
 loop() : Arduino will interpret the DTMF module output and position the Servo accordingly
 ========================================================================================================== */
// Setear contraseña
int D1 = 2;
int D2 = 2;
int D3 = 1;
int D4 = 2;

// Definir pin de Fade
//#define FadePin 9

void setup()
{
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 

  pinMode(STQ, INPUT);
  pinMode(Q4, INPUT);
  pinMode(Q3, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q1, INPUT);
}

void loop()
{

while(getResponse()!="RING"){
   mySerial.println("ATA"); //accept call
   delay(1000);
   tone(Ton,450,1000);
   noTone(Ton);
}
  
  int myDtmf;
  //if (Tonos()) {
  Serial.println("Auntenticando ...");
  if (Autenticacion()) {
    FadeIn();
    Serial.println("Se da el pase al locutor, terminar presionar 0 ");
    while (true) {
      myDtmf = NumInst();
      if (myDtmf == 10) Serial.println(myDtmf); 
      FadeOut(); 
      break; // Detectar cuando se corta la llamada
    }
  }
  //}

}

int NumInst() {
  int detec = 0;
  while (true) {
    if (digitalRead(STQ) == HIGH) {   //When a DTMF tone is detected, STQ will read HIGH for the duration of the tone.
      DTMFread = 0; 
      detec = 1;
      if (digitalRead(Q1) == HIGH) {  //If Q1 reads HIGH, then add 1 to the DTMFread variable
        DTMFread = DTMFread + 1;
      }
      if (digitalRead(Q2) == HIGH) {  //If Q2 reads HIGH, then add 2 to the DTMFread variable
        DTMFread = DTMFread + 2;
      }
      if (digitalRead(Q3) == HIGH) {  //If Q3 reads HIGH, then add 4 to the DTMFread variable
        DTMFread = DTMFread + 4;
      }
      if (digitalRead(Q4) == HIGH) {  //If Q4 reads HIGH, then add 8 to the DTMFread variable
        DTMFread = DTMFread + 8;
      }   //Set the servoPosition varaible to the combined total of all the Q1 to Q4 readings. Multiply by 8.5 to amplify the servo rotation.
    }
    if(detec==1){
      while(true){
        if (digitalRead(STQ) == LOW){ 
          detec=0;
          break;
        }
      }
      break;
    }
  }
  return DTMFread;
}


// Play a default number with all possibilities
//dtmf.playDTMF();

// Read DTMF codes one by one and print it on Serial
/*int myDtmf;
 myDtmf = dtmf.getDTMF();
 if(myDtmf != -1) Serial.println(myDtmf);
 delay(80); // to avoid getting repeated output.*/


boolean Tonos() { //
  int rep = 0;
  int outDTMF;
  int m = 0;

  while (true) {
    // outDTMF = dtmf.getDTMF();
    if (outDTMF == 0) ++rep; // Detectar Ring
    if (outDTMF == -1) return false; 
    break; // Detectar Silencio
    if (rep == 5) return true; 
    break;
    delay(80);
  }
}

boolean Autenticacion() {
  int exito = 0;
  int outDtmf;

  outDtmf = NumInst();
  Serial.println(outDtmf);
  if (outDtmf == D1) ++exito;

  outDtmf = NumInst();
  Serial.println(outDtmf);
  if (outDtmf == D2) ++exito;

  outDtmf = NumInst();
  Serial.println(outDtmf);
  if (outDtmf == D3) ++exito;


  outDtmf = NumInst();
  Serial.println(outDtmf);
  if (outDtmf == D4) ++exito;


  if (exito == 4) return true;
  else return false;
}

void FadeIn()
{
  for (int i=0; i<100; i++) {
    Serial.print("Increasing, i = ");
    Serial.println(i, DEC);
    pot.increase(1);
    pot2.decrease(1);
    delay(200);
  }  
  //  // fade in from min to max in increments of 5 points:
  //  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
  //    // sets the value (range from 0 to 255):
  //    analogWrite(FadePin, fadeValue);
  //    // wait for 30 milliseconds to see the dimming effect
  //    delay(30);
  //  }
}
void FadeOut()
{
    for (int i=0; i<100; i++) {
    Serial.print("Decreasing, i = ");
    Serial.println(i, DEC);
    pot.decrease(1);
    pot2.increase(1);
    delay(200);
  }
//  // fade out from max to min in increments of 5 points:
//  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
//    // sets the value (range from 0 to 255):
//    analogWrite(FadePin, fadeValue);
//    // wait for 30 milliseconds to see the dimming effect
//    delay(30);
//  }
}

String getResponse() {
    String response = "";
    while(mySerial.available()!=0){
        response += mySerial.read();
    }
    return response;
}

