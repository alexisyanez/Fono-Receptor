/* ==================================================================================
 MCI Electronics www.olimex.cl  
 
 Archivo:  Test_3GBee_TIC.ino 
 
 Descripción: Realiza la lectura de un sensor LDR y si pasa el umbral envía un SMS
 a un celular definido.
 
 Dispositivo: XBee IO Pro V2 (Arduino compatible)
              TIC (Arduino compatible)
 
 Microcontrolador: ATmega32U4
 
 ==================================================================================
 //  Ver | dd mmm yyyy | Author  | Description
 // =====|=============|========|=====================================================
 // 1.00 | 19 MAY 2016 | D.M   | First release 
 // ==================================================================================*/ 


//#define PINPWR 12  // XBee IO Pro V2
#define PINPWR 11  // TIC

//#define RL 8  // Relé XBee IO Pro V2
#define RL 7  // Relé TIC

char bufferAnswer[200];
char *pEnd;
char *pStart;
int counter = 0;
int ldrSensor;
boolean alerta = LOW;

void setup()  
{
  Serial.begin(115200);
  Serial1.begin(115200);
  
  pinMode(RL, OUTPUT);
}

void loop()
{
  static int state = 1;
  
  ldrSensor = analogRead(A0);
  if(ldrSensor > 800)
  {
    digitalWrite(RL, HIGH);
    alerta = HIGH;
  }
  else
  {
    digitalWrite(RL, LOW);
  }
  
  switch(state)
  {
    case 1:
      if(ModemON() == HIGH)
      {
        state = 2;
      }

    break;
    
    case 2:
      if(ConfigSMS() == HIGH)
      {
        state = 3;
      }

    break;
    
    case 3:
    
    if(alerta == HIGH)  // umbral asignado
    {
      if(SendCommand("AT+CMGS=\"+569XXXXXXXX\"", ">", 3000, HIGH) == '1')  // <---- Número para envío de alerta +569XXXXXXXX 
      {
        Serial1.print("Alerta, nivel excedido");
        Serial1.write(26);    
        alerta = LOW;
        Serial.println("Alerta, nivel excedido");
        delay(1000);
      }
      else
      {
        state = 1;
      }  
    }
    else
    {
      Serial.println("Level OK");
      delay(1000);
    }
   break; 
  }
}

// ----------------------------------
boolean ModemON()
{ 
  boolean onState;
  pinMode(PINPWR, OUTPUT); 
  delay(1000);
  digitalWrite(PINPWR, HIGH);
  if(SendCommand("AT", "OK", 1000, HIGH) == '1')
  {
    Serial.println("Modem ON");
    onState = HIGH;
  }
  else
  {
    Serial.println("Modem OFF");
    onState = LOW;
  }  
  return onState;  
}

// ----------------------------------
boolean ConfigSMS()
{ 
  boolean onState;
  if(SendCommand("AT+CMGF=1", "OK", 1000, HIGH) == '1')
  {
    if(SendCommand("AT+CSCS=\"GSM\"", "OK", 1000, HIGH) == '1')
    {
      Serial.println("Config SMS OK");
      onState = HIGH;
    }
  }
  else
  {
    Serial.println("Config SMS Fail");
    onState = LOW;
  }  
  return onState;  
}

// ----------------------------------
char SendCommand(char *command, char *ack, unsigned long time, boolean resp)
{
  char *pTemptBuff = bufferAnswer;
  unsigned long timeOut = millis() + time;
  char rCode = '0';
  
  Serial1.flush();
  Serial1.println(command); 
  //Serial.println(command);  // solo debug
  
  if(resp == HIGH)
  {    
    do
      {
        if(Serial1.available() > 0)
          {
            char dataIn = Serial1.read();
            Serial.print(dataIn);
            if(pTemptBuff < (bufferAnswer + sizeof(bufferAnswer))-1)
            {
              *(pTemptBuff++) = dataIn;
              *pTemptBuff = '\0';
              timeOut = millis() + time;
              if(strstr(bufferAnswer, ack))
              {
                rCode = '1';
                break;
              }
            }
            else
            {
              pTemptBuff = bufferAnswer;
            }
          }
      }while(timeOut > millis());
  }
  
  if(timeOut < millis())
    {
      rCode = '2';
      Serial.println("  Time out");
    } 
  //Serial.println(rCode,DEC);   // solo debug
  return rCode;
}
