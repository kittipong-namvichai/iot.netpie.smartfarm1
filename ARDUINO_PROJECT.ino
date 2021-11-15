//---------- DHT11 device
#include <DHT.h>
#include "DFRobot_PH.h" 

#define DHT11PIN 2
#define DHTTYPE DHT11
#define PH_PIN A4


DHT dht(DHT11PIN, DHTTYPE);
DFRobot_PH ph;

enum
{
  RELAY1 = 4,
  RELAY2,
  RELAY3,
  RELAY4,
  RELAY5,
  RELAY6,
  RELAY7,
  RELAY8
};

enum
{
  STATE_RELAY1,
  STATE_RELAY2,
  STATE_RELAY3,
  STATE_RELAY4,
  STATE_RELAY5,
  STATE_RELAY6,
  STATE_RELAY7,
  STATE_RELAY8
};

enum
{
  AI0,  // wind Speed sensor
  AI1,  // wind Direction sensor
  AI2,  // soid moisture sensor
  AI3,  // light sensor
  AI4,  // ph sensor
  AI5   // unused pin
};

enum Mode
{
  AUTO = 0,
  MANUAL,
  SETUP,
  NONE
};

//********** Global Variables **********/
long lastMsg            = 0;
String inputString      = "";     // a String to hold incoming data
bool stringComplete     = false;  // whether the string is complete
bool stateRelays[8];
float h,tc,tf;

//----------***** Analog *****----------//
float voltages[5];
int analogs[5];

enum Mode lastMode;
enum Mode mode;

int incomingByte = 0; // for incoming serial data

void setup() 
{
  // initialize serial:
  Serial.println("********** Initialize serial **********");
  Serial.begin(9600);
  Serial.println("Baudrate: 9600");
  // reserve 10 bytes for the inputString:
  inputString.reserve(10);

  //---------- sets the digital pins as outputs----------//
  pinMode(RELAY1 , OUTPUT);    
  pinMode(RELAY2 , OUTPUT);    
  pinMode(RELAY3 , OUTPUT);    
  pinMode(RELAY4 , OUTPUT);    
  pinMode(RELAY5 , OUTPUT);    
  pinMode(RELAY6 , OUTPUT);    
  pinMode(RELAY7 , OUTPUT);    
  pinMode(RELAY8 , OUTPUT);   

  String pin = "Pin";
  Serial.println("RELAY1 = " + pin+ String(RELAY1));
  Serial.println("RELAY2 = " + pin+ String(RELAY2));
  Serial.println("RELAY3 = " + pin+ String(RELAY3));
  Serial.println("RELAY4 = " + pin+ String(RELAY4));
  Serial.println("RELAY5 = " + pin+ String(RELAY5));
  Serial.println("RELAY6 = " + pin+ String(RELAY6));
  Serial.println("RELAY7 = " + pin+ String(RELAY7));
  Serial.println("RELAY8 = " + pin+ String(RELAY8));

  Serial.println("********** Initial all outputs to OFF (HIGH State) **********");
  digitalWrite(RELAY1 , HIGH); 
  digitalWrite(RELAY2 , HIGH); 
  digitalWrite(RELAY3 , HIGH); 
  digitalWrite(RELAY4 , HIGH); 
  digitalWrite(RELAY5 , HIGH); 
  digitalWrite(RELAY6 , HIGH); 
  digitalWrite(RELAY7 , HIGH); 
  digitalWrite(RELAY8 , HIGH);

  Serial.println(F("DHT11 test!"));
  dht.begin();
  ph.begin();
}

void ReadAndReportDHT11()
{
  h = dht.readHumidity();
  tc = dht.readTemperature();
  tf = dht.readTemperature(true);
  
  Serial.print('\n');
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print("%,  ");
  Serial.print("Temperature = ");
  Serial.print(tc);
  Serial.print("°C, ");
  Serial.print(tf);
  Serial.println("°F");
}

void ReadDHT11()
{
  h = dht.readHumidity();
  tc = dht.readTemperature();
  tf = dht.readTemperature(true);
}

void loop() 
{
    //print the string when a newline arrives:
     

    if(lastMode != mode)
    {
      lastMode = mode;
      Serial.println("********** MODE CHANGED **********");
      CheckMode();
    }

    if(mode == AUTO)
    {
      long now = millis();
      if((now - lastMsg) > 2000)
      {
        lastMsg = now;
        ReadAllSensors();
      }
    }
    
    else if(mode == SETUP)
    {
      
    }

    if (stringComplete) 
    {
      Serial.println(inputString);
      // clear the string:
      inputString = "";
      stringComplete = false;
    } 
}

void CheckMode()
{
  if(mode == AUTO)
    Serial.println("********** IN AUTO MODE **********");
  else if(mode == MANUAL)
    Serial.println("********** IN MANUAL MODE **********");
  else if(mode == SETUP)
    Serial.println("********** IN SETUP MODE **********");
   else if(mode == NONE)
    Serial.println("********** IN NONE MODE **********");
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/

void serialEvent() 
{
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if(inputString == "a")
    {
      mode = AUTO;
      Serial.println("********** RECEIVED AUTO MODE COMMAND **********");
    }
    else if(inputString == "m")
    {
      mode = MANUAL;
      Serial.println("********** RECEIVED MANUAL MODE COMMAND **********"); 
    }
    else if(inputString == "s")
    {
      mode = SETUP;
      Serial.println("********** RECEIVED SETUP MODE COMMAND **********");
    }

    else if(inputString == "c")
    {
      CheckMode();
    }

    else if(inputString == "on1")
    {
      if(!stateRelays[STATE_RELAY1])
      {
          stateRelays[STATE_RELAY1] = true;
          digitalWrite(RELAY1 , LOW);
      }
    }
    else if(inputString == "on2")
    {
      if(!stateRelays[STATE_RELAY2])
        {
          stateRelays[STATE_RELAY2] = true;
          digitalWrite(RELAY2 , LOW);
        }  
    }

    else if(inputString == "on3")
    {
      if(!stateRelays[STATE_RELAY3])
        {
          stateRelays[STATE_RELAY3] = true;
          digitalWrite(RELAY3 , LOW);
        }  
    }

    else if(inputString == "on4")
    {
      if(mode == MANUAL)
      {
        
      }
      if(!stateRelays[STATE_RELAY4])
      {
          stateRelays[STATE_RELAY4] = true;
          digitalWrite(RELAY4 , LOW);
      }  
    }

    else if(inputString == "on5")
    {
      if(mode == MANUAL)
      {
        
      }
      if(!stateRelays[STATE_RELAY5])
      {
          stateRelays[STATE_RELAY5] = true;
          digitalWrite(RELAY5 , LOW);
      }  
    }

    else if(inputString == "on6")
    {
      if(mode == MANUAL)
      {
        
      }
      if(!stateRelays[STATE_RELAY6])
      {
          stateRelays[STATE_RELAY6] = true;
          digitalWrite(RELAY6 , LOW);
      }  
    }

    else if(inputString == "on7")
    {
      if(mode == MANUAL)
      {
         
      }
      if(!stateRelays[STATE_RELAY7])
      {
          stateRelays[STATE_RELAY7] = true;
          digitalWrite(RELAY7 , LOW);
      } 
    }

    else if(inputString == "on8")
    {
      if(mode == MANUAL)
      {
        
      }
      if(!stateRelays[STATE_RELAY8])
      {
          stateRelays[STATE_RELAY8] = true;
          digitalWrite(RELAY8 , LOW);
      }  
    }

    else if(inputString == "off1")
    {
      if(stateRelays[STATE_RELAY1])
      {
          stateRelays[STATE_RELAY1] = false;
          digitalWrite(RELAY1 , HIGH);
      }
    }

    else if(inputString == "off2")
    {
      if(stateRelays[STATE_RELAY2])
      {
        stateRelays[STATE_RELAY2] = false;
        digitalWrite(RELAY2 , HIGH);
      }
    }

    else if(inputString == "off3")
    {
      if(mode == MANUAL)
      {
        
      }
      if(stateRelays[STATE_RELAY3])
      {
          stateRelays[STATE_RELAY3] = false;
          digitalWrite(RELAY3 , HIGH);
      }
    }

    else if(inputString == "off4")
    {
      if(mode == MANUAL)
      {
        
      }
      if(stateRelays[STATE_RELAY4])
      {
          stateRelays[STATE_RELAY4] = false;
          digitalWrite(RELAY4 , HIGH);
      }
    }

    else if(inputString == "off5")
    {
      if(mode == MANUAL)
      {
        
      }
      if(stateRelays[STATE_RELAY5])
      {
          stateRelays[STATE_RELAY5] = false;
          digitalWrite(RELAY5 , HIGH);
      }
    }

    else if(inputString == "off6")
    {
      if(mode == MANUAL)
      {
        
      }
      if(stateRelays[STATE_RELAY6])
      {
          stateRelays[STATE_RELAY6] = false;
          digitalWrite(RELAY6 , HIGH);
      }
    }

    else if(inputString == "off7")
    {
      if(mode == MANUAL)
      {
        
      }
      if(stateRelays[STATE_RELAY7])
      {
          stateRelays[STATE_RELAY7] = false;
          digitalWrite(RELAY7 , HIGH);
      }
    }

    else if(inputString == "off8")
    {
      if(mode == MANUAL)
      {
        
      }
      if(stateRelays[STATE_RELAY8])
      {
          stateRelays[STATE_RELAY8] = false;
          digitalWrite(RELAY8 , HIGH);
      }
    }

    else if(inputString == "offall")
    {
      if(mode == MANUAL)
      {
        digitalWrite(RELAY1 , HIGH);
        digitalWrite(RELAY2 , HIGH);
        digitalWrite(RELAY3 , HIGH);
        digitalWrite(RELAY4 , HIGH);
        digitalWrite(RELAY5 , HIGH);
        digitalWrite(RELAY6 , HIGH);
        digitalWrite(RELAY7 , HIGH);
        digitalWrite(RELAY8 , HIGH);
      }
    }

    else if(inputString == "onall")
    {
      if(mode == MANUAL)
      {
        digitalWrite(RELAY1 , LOW);
        digitalWrite(RELAY2 , LOW);
        digitalWrite(RELAY3 , LOW);
        digitalWrite(RELAY4 , LOW);
        digitalWrite(RELAY5 , LOW);
        digitalWrite(RELAY6 , LOW);
        digitalWrite(RELAY7 , LOW);
        digitalWrite(RELAY8 , LOW);
      }
    }
  
    if(inChar == '\n') 
    {
      stringComplete = true;
      //Serial.println("********** RECEIVED COMMAND " + inputString + "**********");
    } 
  }
}


float ConvertToVoltage(float sensorValue)
{
  return sensorValue * (5.0 / 1023.0);
}


void ReadAllSensors()
{

  h = dht.readHumidity();
  tc = dht.readTemperature();
  tf = dht.readTemperature(true);
  
  analogs[AI0] = analogRead(A0);
  analogs[AI1] = analogRead(A1);
  analogs[AI2] = analogRead(A2);
  analogs[AI3] = analogRead(A3);
  analogs[AI4] = analogRead(A4); //ph sensor

  voltages[AI0] = (analogs[AI0] / 1023.00)* 5.00;
  float windSpeed = voltages[AI0] / 1.6 * 32.4;
  Serial.print  (windSpeed );           // wind speed 
  Serial.print  (",");

  float windDirect =  (analogs[AI1] / (1023.00 - 4)) * 360.00;
  Serial.print  (windDirect);   // wind direction
  Serial.print  (",");


  Serial.print  (analogs[AI2]);   // light
  Serial.print  (",");

  voltages[AI3] = (analogs[AI3] / 1023.00)* 5.00;
  float soilMoisture = 100.00 - ((analogs[AI3] / 1023.00) * 100.00);
  Serial.print  (soilMoisture);
  Serial.print  (",");

  voltages[AI4] = analogs[AI4] / 1024.00 * 5000.0; // ph 
  float phValue = ph.readPH(voltages[AI4],tc);
  Serial.print  (analogs[AI4]);
  Serial.print  (",");
  
  Serial.print  (h);
  Serial.print  (",");
  
  Serial.print  (tc);
  Serial.print  (",");
  
  Serial.println(tf);
}
