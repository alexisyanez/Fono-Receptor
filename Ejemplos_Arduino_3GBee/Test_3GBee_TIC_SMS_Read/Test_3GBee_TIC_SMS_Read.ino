/* ==================================================================================
 MCI Electronics www.olimex.cl  
 
 Archivo:  Test_3GBee_TIC.ino 
 
 Descripción: Realiza la lectura de un mensaje de texto con el cual maneja el relé
 
 Dispositivo: TIC (Arduino compatible)
              XBee IO Pro V2 (Arduino compatible)
 
 Microcontrolador: ATmega32U4
 
 ==================================================================================
 //  Ver | dd mmm yyyy | Author  | Description
 // =====|=============|========|=====================================================
 // 1.00 | 19 MAY 2016 | D.M   | First release 
 // ==================================================================================*/ 


//#define PINPWR 12  // XBee IO Pro V2
#define PINPWR 11  // TIC

//#define PINPWR 8  // Relé XBee IO Pro V2
#define RL1 7  // Relé TIC

char bufferAnswer[200];
char *pEnd;
char *pStart;
int counter = 0;
int ldrSensor;

void setup()  
{
  Serial.begin(115200);
  Serial1.begin(115200);
  
  pinMode(RL1, OUTPUT);
}

void loop()
{
  static int state = 1;
  
  switch(state)
  {
    case 1:
    {
      if(ModemON() == HIGH)
      {
        state = 2;
      }
      else 
      {
        counter++;
      }
    break;
    }
    
    case 2:
    {
      if(ConfigSMS() == HIGH)
      {
        state = 3;
      }
      else 
      {
        counter++;
      }
    break;
    }
    
    case 3:
    {
      if(FlagSMS() == HIGH)
      {
        state = 4;
      }
      break;
    }
    
    case 4:
    {
      if(ReadSMS() == HIGH)
      {
        state = 3;
      }
      break;
    }
    
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
  if(SendCommand("AT+CMGF=1", "OK", 1000, HIGH) == '1') // Modo Texto
  {
    if(SendCommand("AT+CSCS=\"GSM\"", "OK", 1000, HIGH) == '1') // selecciona el set de caracteres utilizados por el modem para enviar el SMS
    {
      if(SendCommand("AT+CNMI=1,1", "OK", 1000, HIGH) == '1')
      {
        Serial.println("Config SMS OK");
        onState = HIGH;
      }
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

// ----------------------------------
boolean ReadSMS()
{
  boolean onState = LOW;
  char *puntero;
  
  if(SendCommand("AT+CMGR=1", "OK", 1000, HIGH) == '1')
  {
    if(strstr(bufferAnswer, "+56982805725"))  // <------ Ingresar número válido
    {
      if(strstr(bufferAnswer, ">R1ON<"))
      {
        digitalWrite(RL1, HIGH);
        SendCommand("AT+CMGD=1,4", "OK", 1000, HIGH);
        onState = HIGH;
      }
      else if(strstr(bufferAnswer, ">R1OFF<"))
      {
        digitalWrite(RL1, LOW);
        SendCommand("AT+CMGD=1,4", "OK", 1000, HIGH);
        onState = HIGH;
      }
      else
      {
        Serial.println("Error de mensaje");
      }
    }
    else
    {
      Serial.println("Identificacion no valida");  // Identificación no válida del número celular
      SendCommand("AT+CMGD=1,4", "OK", 1000, HIGH);
    }
  }
  else
  {
    Serial.println("Sin mensaje");
  }  
  return onState;  
}

// ----------------------------------
boolean FlagSMS()
{
  boolean onState = LOW;
  char *pTemptBuff = bufferAnswer;
  unsigned long time = millis() + 1000;
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
          time = millis() + 1000;
          if(strstr(bufferAnswer, "+CMTI: \"SM\""))
          {
            onState = HIGH;
            break;
          }
        }
        else
        {
          pTemptBuff = bufferAnswer;
        }
      }
  }while(time > millis());
  
  return onState;
}
  

