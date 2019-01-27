#include <DigiPotX9Cxxx.h>
#include <SoftwareSerial.h>
#include "pitches.h"

SoftwareSerial mySerial(13, 12); //SIM800L Tx & Rx is connected to Arduino #13 & #12

DigiPot pot(7,8,6);
DigiPot pot2(2,3,4);

#define  PASS "12345"
//#define  BUFF ""
//byte DTMFread; 
//const int STQ = 3;        // Attach DTMF Module STQ Pin to Arduino Digital Pin 3
//const int Q4 = 4;        // Attach DTMF Module Q4  Pin to Arduino Digital Pin 4
//const int Q3 = 5;        // Attach DTMF Module Q3  Pin to Arduino Digital Pin 5
//const int Q2 = 6;        // Attach DTMF Module Q2  Pin to Arduino Digital Pin 6
//const int Q1 = 7;        // Attach DTMF Module Q1  Pin to Arduino Digital Pin 7
String BUFF;
const int Ton = 11; 
const int voice= 9;

void setup()
{
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  delay(3000);
  Serial.println(mySerial.readString());
  
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  delay(3000);
  Serial.println(mySerial.readString());

  mySerial.println("ATS0=2"); //Signal quality test, value range is 0-31 , 31 is the best
  delay(3000);
  Serial.println(mySerial.readString());
    
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  delay(3000);
  Serial.println(mySerial.readString());
  
  mySerial.println("AT+CREG=1"); //Check whether it has registered in the network
  delay(1000);
  Serial.println(mySerial.readString());
  
  mySerial.println("AT+DDET=1"); //Check whether it has registered in the network
  delay(3000);
  Serial.println(mySerial.readString());
   
  mySerial.println("AT+CREG?");
  delay(2000);
  Serial.println(mySerial.readString());
  
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  delay(3000);
  Serial.println(mySerial.readString());

  pinMode(Ton,OUTPUT);
  pinMode(voice,OUTPUT);
  
  FadeIn();
  
}

void loop()
{
  if((mySerial.readString()).indexOf("RING") != -1){
    delay(3000);
    //tone(Ton,NOTE_E4,1000);
    //noTone(Ton);
    Serial.println("Auntenticando ...");
    if (Autenticacion()) {
        digitalWrite(9,HIGH);
      
       // tone(Ton,NOTE_C4,1000);
       // tone(Ton,NOTE_G4,1000);
       // noTone(Ton);
        
        Serial.println("Contraseña Correcta!!");
        FadeOut();
        Serial.println("Se da el pase al locutor, terminar presionar 0 ");
        while(true){
          if((mySerial.readString()).indexOf("+DTMF:0") != -1){
            digitalWrite(9,LOW);
            FadeIn();
            break;         
          }
        }
    }
  }
}


void FadeIn()
{
  for (int i=0; i<25; i++) {
    Serial.print("Increasing, i = ");
    Serial.println(i, DEC);
    pot.increase(4);
    pot2.increase(4);
    delay(200);
  }  
}

void FadeOut()
{
    for (int i=0; i<25; i++) {
    Serial.print("Decreasing, i = ");
    Serial.println(i, DEC);
    pot.decrease(4);
    pot2.decrease(4);
    delay(200);
  }
}

String getResponse() {
    String response = "";
    while(mySerial.available()){
        response += mySerial.read();
    }
    return response;
}

boolean Autenticacion() {
int j=0;
for (int i=0;i<6;++i){
  while(true){
    if((mySerial.readString()).indexOf("+DTMF:"+PASS[i]) != -1){
      Serial.println(mySerial.readString());
      ++j;
      break; 
    }
  }
}
    
if(j=5) {
  return true;}
  
else{
  return false;}
     
}
