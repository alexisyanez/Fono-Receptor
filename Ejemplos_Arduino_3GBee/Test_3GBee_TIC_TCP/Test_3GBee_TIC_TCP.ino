/* ==================================================================================
 MCI Electronics www.olimex.cl  
 
 Archivo:  Test_3GBee_TIC.ino 
 
 Descripción: Realiza la lectura de un sensor y lo reporta a un servidor TCP
 
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
int valueSensor;

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

    break;
    }
    
    case 2:
    {
      if(ModemConnect() == 7)
      {
        state = 3;
      }

    break;
    }
    
    case 3:
    {
      valueSensor = analogRead(A0);
      Serial1.println(valueSensor);
      Serial.println(valueSensor);
      delay(10000);
      state = 4;
 
      break;
    }
    
    case 4:
    {
      if(CloseServer() == HIGH)
      {
        state = 2;
      }
 
      break;
    }
  
  }
  
  Serial.println("");
  Serial.print("Valor del sensor: ");
  Serial.println(analogRead(A0));
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
int ModemConnect()
{
  static int var = 1;
  int state = 1;
  
  switch (var) 
  {
    case 1:
      Serial.println();
      Serial.print("Echo off ");
      if(SendCommand("ATE0","OK", 5000, HIGH) == '1')
      {
        var = 2;
      }
      clearBuff(bufferAnswer);
    break;
      
    case 2:
      Serial.println();
      Serial.print("AT command ");
      if(SendCommand("AT","OK", 5000, HIGH) == '1')
      {
        var = 3;
      }
      clearBuff(bufferAnswer);
    break;
      
    case 3:
      Serial.println();
      Serial.print("Set APN ");
      if(SendCommand("AT+CGDCONT=1,\"IP\",\"web.tmovil.cl\"","OK", 5000, HIGH) == '1')  // AT+CGDCONT=1,"IP","web.tmovil.cl"
      {
        var = 4;
      }
      clearBuff(bufferAnswer);
    break;
      
    case 4:
      Serial.println();
      Serial.print("Network connecting ");
      if(SendCommand("AT#SGACT=1,1","#SGACT:", 8000, HIGH) == '1')
      {
        var = 5;
      }
      clearBuff(bufferAnswer);
    break;
      
    case 5:
      Serial.println();
      Serial.print("No flow control ");
      if(SendCommand("AT&K0","OK", 5000, HIGH) == '1')
      {
        var = 6;
      }
      clearBuff(bufferAnswer);
    break;
      
    case 6:
      Serial.println();
      Serial.print("Coonnecting Server ");
      if(SendCommand("AT#SD=1,0,1836,200.113.93.142,0,0,0","CONNECT", 8000, HIGH) == '1') // Conectar al server TCP indicando puerto e IP
      {
        var = 6;
        state = 7;
      }
      clearBuff(bufferAnswer);
    break;
      
    default: 
    break;
      // if nothing else matches, do the default
      // default is optional
  }
  
  return state;
}

// ----------------------------------
void clearBuff(char *dirBuff)
{
  *dirBuff = '\0';
}

boolean CloseServer()
{
  boolean onState = LOW;
  Serial1.print("+++"); 
  delay(3000);
  if(SendCommand("AT#SH=1", "OK", 1000, HIGH) == '1')
  {
    onState == HIGH;
  }
  else
  {
    onState == LOW;    
  }
}

// ----------------------------------
char SendCommand(char *command, char *ack, unsigned long time, boolean resp) // resp HIGH, espera respuesta
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

