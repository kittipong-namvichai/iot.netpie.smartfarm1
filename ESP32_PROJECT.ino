//  PROGRAMMER        : KITTIPONG NAMVICHAI
//  LAST MODIFICATION : 22-OCT-2021
//
//////////////////////////////////////////////////////////////////////////

//********** IMPORT LIBRARIES**********//
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

//********** Define serial port2 **********// 
#define RXD2 16
#define TXD2 17
#define SIMULATION false //true = simulation sensor values, false = readind values from sensors

enum Mode
{
  AUTO = 0,
  MANUAL,
  SETUP,
  NONE
};

enum
{
  RELAY1 = 0,
  RELAY2,
  RELAY3,
  RELAY4,
  RELAY5,
  RELAY6,
  RELAY7,
  RELAY8
};

enum STATE
{
  IDLE_CONTROL,
  TEMP_CONTROL,
  MOISTURE_CONTROL,
  LIGHT_CONTROL,
  LIGHT_CONTROL_OFF
};

Mode runMode = AUTO;

STATE CurrentState = IDLE_CONTROL;
STATE CurrentStateMem;
STATE NextState = IDLE_CONTROL;

//********** GOBAL CONST VARIABLES **********//
const char* ssid          = "Your WiFi SSID";           		   		// WiFi SSID
const char* password      = "Your WiFi Password";                  		// WiFi Password
const char* mqtt_server   = "broker.netpie.io";                     	// Netpie server name
const int   mqtt_port     = 1883;                                   	// Server Port
const char* mqtt_Client   = "Your Client ID from NETPIE2020"; 			// Client ID
const char* mqtt_username = "Your Client Token ID from NETPIE2020"; 	// Token ID
const char* mqtt_password = "Your Client Secret ID from NETPIE2020"; 	// Secret ID

//********** GOBAL VARIABLES **********// 
int temperature           = 0;
int light                 = 0;
int analog_value          = 0;
char msg[500]                ;
char msg2[500]               ;
long lastMsg              = 0;

String Temperature = "";
String Humidity    = "";
String Light              = "";
String SoilMoisture       = "";
String WindSpeed          = "";
String WindDirect         = "";

String inputString        = "";     
char str[100];
bool stringComplete       = false;  

String SetTemperature     = "";
String SetHumidity        = "";
String SetSoilMoisture    = "";
String SetLight           = "";

float targetTemperature;
float targetHumidity;
float targetSoilMoisture;
float targetLight;

bool relayStates[8]           ;
int _delay = 300;
bool condition1;
bool condition2;
bool condition3;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Attempting NETPIE2020 connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) 
    {
      Serial.println("NETPIE2020 connected");
      client.subscribe("@msg/relay1");
      client.subscribe("@msg/relay2");
      client.subscribe("@msg/relay3");
      client.subscribe("@msg/relay4");
      client.subscribe("@msg/relay5");
      client.subscribe("@msg/relay6");
      client.subscribe("@msg/relay7");
      client.subscribe("@msg/relay8");
      delay(2000);
      client.subscribe("@msg/mode");
      client.subscribe("@msg/settemperature");
      client.subscribe("@msg/sethumidity");
      client.subscribe("@msg/setsoilmoisture");
      client.subscribe("@msg/setlight");
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  
  Serial.println(message);
  
  if(String(topic) == "@msg/relay1") 
  {
    if (message == "on")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay1\" : \"on\"}}");
      Serial2.println("on1"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay1\" : \"off\"}}");
      Serial2.println("off1"); 
    }
  } 

  if(String(topic) == "@msg/relay2") 
  {
    if (message == "on")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay2\" : \"on\"}}");
      Serial2.println("on2"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay2\" : \"off\"}}");
      Serial2.println("off2"); 
    }
  } 

  if(String(topic) == "@msg/relay3") 
  {
    if (message == "on")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay3\" : \"on\"}}");
      Serial2.println("on3"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay3\" : \"off\"}}");
      Serial2.println("off3"); 
    }
  } 

  if(String(topic) == "@msg/relay4") 
  {
    if (message == "on")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay4\" : \"on\"}}");
      Serial2.println("on4"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay4\" : \"off\"}}");
      Serial2.println("off4"); 
    }
  } 

  if(String(topic) == "@msg/relay5") 
  {
    if (message == "on")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay5\" : \"on\"}}");
      Serial2.println("on5"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay5\" : \"off\"}}");
      Serial2.println("off5"); 
    }
  } 

  if(String(topic) == "@msg/relay6") 
  {
    if (message == "on"){
      client.publish("@shadow/data/update", "{\"data\" : {\"relay6\" : \"on\"}}");
      Serial2.println("on6"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay6\" : \"off\"}}");
      Serial2.println("off6"); 
    }
  } 

  if(String(topic) == "@msg/relay7") 
  {
    if (message == "on")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay7\" : \"on\"}}");
      Serial2.println("on7"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay7\" : \"off\"}}");
      Serial2.println("off7"); 
    }
  } 

  if(String(topic) == "@msg/relay8") 
  {
    if (message == "on")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay8\" : \"on\"}}");
      Serial2.println("on8"); 
    }
    else if (message == "off") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"relay8\" : \"off\"}}");
      Serial2.println("off8"); 
    }
  } 

  if(String(topic) == "@msg/mode") 
  {
    if (message == "auto")
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"mode\" : \"auto\"}}");
      runMode = AUTO;
      Serial2.println("a"); 
    }
    else if (message == "manual") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"mode\" : \"manual\"}}");
      runMode = MANUAL;
      Serial2.println("m"); 
    }
    else if (message == "setup") 
    {
      client.publish("@shadow/data/update", "{\"data\" : {\"mode\" : \"setup\"}}");
      runMode = SETUP;
      Serial2.println("s"); 
    }
  } 

  if(String(topic) == "@msg/settemperature")
  {
    targetTemperature = message.toFloat();
    Serial2.println(message);
  }

  if(String(topic) == "@msg/sethumidity")
  {
    targetHumidity = message.toFloat();
    Serial2.println(message);
  }

  if(String(topic) == "@msg/setsoilmoisture")
  {
    targetSoilMoisture = message.toFloat();
    Serial2.println(message);
  }

  if(String(topic) == "@msg/setlight")
  {
    targetLight = message.toFloat();
    Serial2.println(message);
  }
}



void serialEvent() 
{
  while (Serial2.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    inputString += inChar;

    if(inChar == '\n') 
      stringComplete = true;
  }
}

void TurnOffAllOutputs()
{
  Serial2.println("off1");
  delay(_delay);  
  Serial2.println("off2");
  delay(_delay);  
  Serial2.println("off3");
  delay(_delay);  
  Serial2.println("off4");
  delay(_delay);  
  Serial2.println("off5");
  delay(_delay);  
  Serial2.println("off6");
  delay(_delay);  
  Serial2.println("off7");
  delay(_delay);  
  Serial2.println("off8"); 
  delay(_delay);  
}

void CheckForChangeState()
{
  bool enableCondition1 = targetTemperature > 0 && targetHumidity > 0;
  bool enableCondition2 = targetSoilMoisture > 0;
  bool enableCondition3 = targetLight > 0;
  if(enableCondition3 && Light.toFloat() >= targetLight)
  {
    NextState = LIGHT_CONTROL;
  }
  else if(enableCondition3 && Light.toFloat() < targetLight)
  {
    NextState = LIGHT_CONTROL_OFF;
  }
  else if(enableCondition2 && SoilMoisture.toFloat() >= targetSoilMoisture)
  {
    NextState = MOISTURE_CONTROL;
  }
  else if(enableCondition1 && Temperature.toFloat() >= targetTemperature && Humidity.toFloat() >= targetHumidity)
  {
    NextState = TEMP_CONTROL;
  }
  else
  {
    NextState = IDLE_CONTROL;
  }
}



String GetCurrentState(STATE currentState)
{
  if(currentState == TEMP_CONTROL)
    return "TEMP_CONTROL";
  else if(currentState == MOISTURE_CONTROL)
    return "MOISTURE_CONTROL";
  else if(currentState == LIGHT_CONTROL)
    return "LIGHT_CONTROL";
  else if(currentState == LIGHT_CONTROL_OFF)
    return "LIGHT_CONTROL_OFF";
  else if(currentState == IDLE_CONTROL)
    return "IDLE_CONTROL";
}
void PublishTurnOffAllOutputs()
{
  client.publish("@shadow/data/update", "{\"data\" : {\"relay1\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay2\" : \"off\"}}");
  delay(_delay);  
  client.publish("@shadow/data/update", "{\"data\" : {\"relay3\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay4\" : \"off\"}}");
  delay(_delay);  
  client.publish("@shadow/data/update", "{\"data\" : {\"relay5\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay6\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay7\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay8\" : \"off\"}}");
  delay(_delay); 
}



void TempControlState()
{
  Serial2.println("on1");
  delay(_delay);   
  Serial2.println("on4");
  delay(_delay);  

  Serial2.println("off2");
  delay(_delay); 
  Serial2.println("off3");
  delay(_delay);  
  Serial2.println("off5");
  delay(_delay);  
  Serial2.println("off6");
  delay(_delay);  
  Serial2.println("off7");
  delay(_delay);  
  Serial2.println("off8");
  delay(_delay);  
}

void MoistureControlState()
{
  Serial2.println("on1");
  delay(_delay);  
  Serial2.println("on2");
  delay(_delay);   
  Serial2.println("on5");
  delay(_delay);  
  Serial2.println("on6");
  delay(_delay);  
  Serial2.println("on7");
  delay(_delay);  
  Serial2.println("on8");
  delay(_delay);  

  Serial2.println("off3");
  delay(_delay);  
  Serial2.println("off4");
  delay(_delay); 
}
void LightControlState()
{
  Serial2.println("on2");
  delay(_delay);
  Serial2.println("on3");
  delay(_delay);  
  Serial2.println("on5");
  delay(_delay);  
  Serial2.println("on6");
  delay(_delay);  
  Serial2.println("on7");
  delay(_delay);  
  Serial2.println("on8");
  delay(_delay);  

  Serial2.println("off1");
  delay(_delay);  
  Serial2.println("off4");
  delay(_delay);  
}

void LightControlOffState()
{
  Serial2.println("off2");
  delay(_delay); 
  Serial2.println("off3");
  delay(_delay);  
  Serial2.println("off5");
  delay(_delay);  
  Serial2.println("off6");
  delay(_delay);  
  Serial2.println("off7");
  delay(_delay);  
  Serial2.println("off8");
  delay(_delay);  
}


void PublishTempControlState()
{                
  client.publish("@shadow/data/update", "{\"data\" : {\"relay1\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay4\" : \"on\"}}");
  
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay2\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay3\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay5\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay6\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay7\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay8\" : \"off\"}}");
  delay(_delay); 
}

void PublishMoistureControlState()
{
  client.publish("@shadow/data/update", "{\"data\" : {\"relay1\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay2\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay5\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay6\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay7\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay8\" : \"on\"}}");
  delay(_delay); 

  client.publish("@shadow/data/update", "{\"data\" : {\"relay3\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay4\" : \"off\"}}");
  delay(_delay); 
}

void PublishLightControlState()
{
  client.publish("@shadow/data/update", "{\"data\" : {\"relay2\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay3\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay5\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay6\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay7\" : \"on\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay8\" : \"on\"}}");
  delay(_delay); 

  client.publish("@shadow/data/update", "{\"data\" : {\"relay1\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay4\" : \"off\"}}");
  delay(_delay); 
}

void PublishLightControlOffState()
{
  client.publish("@shadow/data/update", "{\"data\" : {\"relay2\" : \"off\"}}");
  delay(_delay);
  client.publish("@shadow/data/update", "{\"data\" : {\"relay3\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay5\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay6\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay7\" : \"off\"}}");
  delay(_delay); 
  client.publish("@shadow/data/update", "{\"data\" : {\"relay8\" : \"off\"}}");
  delay(_delay); 
}

//********** Separate index array received from arduino **********/./ 
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++)
  {
    if(data.charAt(i)==separator || i==maxIndex)
    {
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() 
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // original 8N1
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) 
  {
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(1000);
      Serial.print(".");
    }
  }
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  // reserve 10 bytes for the inputString:
  inputString.reserve(500);
}

void loop() 
{
  if(!client.connected()) 
    reconnect();
    
  client.loop();

  
    
  String place = "My Farm";
  long now = millis();
  if((now - lastMsg) > 2000)
  {
    lastMsg = now;
    String data = "{\"data\": {\"temperature\":" + String(Temperature) + 
                  ",\"light\":" + String(Light) + 
                  ",\"humidity\":" + String(Humidity) + 
                  ",\"soilmoisture\":" + String(SoilMoisture) + 
                  ",\"windspeed\":" + String(WindSpeed) + 
                  ",\"winddirect\":" + String(WindDirect) + 
                  ",\"settemperature\":" + String(targetTemperature) + 
                  ",\"sethumidity\":" + String(targetHumidity) +
                  ",\"setsoilmoisture\":" + String(targetSoilMoisture) +
                  ",\"setlight\":" + String(targetLight) +
                  ",\"state\":" + String(CurrentState) +"}}";
                         
    Serial.println(data);
    //Serial.println(String(GetCurrentState(CurrentState))); 
    data.toCharArray(msg, (data.length() + 1));
    client.publish("@shadow/data/update", msg);

    if(SIMULATION)
    {
      Light = Light != "10"? "10" : "12";
      Temperature = Temperature != "30"? "30" : "31.5";
      Humidity = Humidity != "60"? "60" : "62.5";
      SoilMoisture = SoilMoisture != "50"? "50" : "50.5";
      WindDirect = WindDirect != "270"? "270" : "360";
      WindSpeed = WindSpeed != "10"? "10" : "15";
    }

    
    CurrentState = NextState;
    switch(CurrentState)
    {
      case IDLE_CONTROL:
        if(CurrentStateMem != CurrentState)
        {
          CurrentStateMem = CurrentState;
          
          TurnOffAllOutputs();
          PublishTurnOffAllOutputs();
        }
        CheckForChangeState();         
        break;
        
      case TEMP_CONTROL:
        if(CurrentStateMem != CurrentState)
        {
          CurrentStateMem = CurrentState;
          
          PublishTempControlState();
          TempControlState();
        }
        CheckForChangeState();
        break;
        
      case MOISTURE_CONTROL:
        if(CurrentStateMem != CurrentState)
        {
          CurrentStateMem = CurrentState;

          PublishMoistureControlState();
          MoistureControlState();
        }
        CheckForChangeState();
        break;
        
      case LIGHT_CONTROL:
        if(CurrentStateMem != CurrentState)
        {
          CurrentStateMem = CurrentState;
          PublishLightControlState();
          LightControlState();          
        }
        CheckForChangeState();
        break;
        
       case LIGHT_CONTROL_OFF:
        if(CurrentStateMem != CurrentState)
        {
          CurrentStateMem = CurrentState;
          PublishLightControlOffState();
          LightControlOffState();          
        }
        CheckForChangeState();
        break;  
    }    
  }

  serialEvent();
    
  if(stringComplete) 
  {
    Serial.println(inputString);     
    WindSpeed     = getValue(inputString,',',0); 
    WindDirect    = getValue(inputString,',',1); 
    Light         = getValue(inputString,',',2);
    SoilMoisture  = getValue(inputString,',',3); 
    Temperature   = getValue(inputString,',',6); 
    Humidity      = getValue(inputString,',',7); 
      
      // clear the string:
      inputString = "";
      stringComplete = false; 
  } 
}
