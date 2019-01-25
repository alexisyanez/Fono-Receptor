#include <DigiPotX9Cxxx.h>
#include <SoftwareSerial.h>

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
void setup()
{
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  //delay(3000);
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
  Serial.println(mySerial.readString());
  
  pinMode(Ton,OUTPUT);


}

void loop()
{
   /* Serial.println("Auntenticando ...");
  if (Autenticacion()) {
      Serial.println("Contraseña Correcta!!");
      FadeIn();
      Serial.println("Se da el pase al locutor, terminar presionar 0 ");
  }*/
  FadeIn();
  FadeOut();
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
for (int i=0;i<5;++i){
  while(true){
    if(getResponse()=="+DTMF:"+PASS[i]){
      ++j;
      break; 
    }
  }
}
    
if(j=4) {
  return true;}
  
else{
  return false;}
     
}
