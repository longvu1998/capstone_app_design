#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h>
#define dirPin 26
#define stepPin 25
#define visensor 34
int i = 0,cooldown = 0,sensor = 0, hitvalue = 0,resetCheck = 0;
const char* ssid = "Advance Precision Target";
const char* password = "12345678";


//Your IP address or domain name with URL path
const char* serverNameTarget = "http://192.168.8.102/target";
const char* serverNameLife = "http://192.168.8.102/life";
const char* serverNameShot = "http://192.168.8.102/shot";
const char* serverNameDown = "http://192.168.8.102/down";
const char* serverNameReset = "http://192.168.8.102/reset";
long target;
long raisenumber;
long hittime;
long downtime;
long reset;

unsigned long previousMillis = 0;
const long interval = 500; 


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void TargetDown()
{   Serial.println("target Down");
    digitalWrite(dirPin, HIGH);
    for (int i = 0; i <= 300; i++)
               {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(900);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(900);
               }
               i++;
}

void TargetUp()
{   Serial.println("target Up");
    digitalWrite(dirPin, LOW);
    for (int i = 0; i <= 300; i++)
               {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(900);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(900);
               }
               i++;  
}

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      target = httpGETRequest(serverNameTarget).toInt();
      raisenumber = httpGETRequest(serverNameLife).toInt();
      hittime = httpGETRequest(serverNameShot).toInt();
      downtime = httpGETRequest(serverNameDown).toInt();
      reset = httpGETRequest(serverNameReset).toInt();
      // save the last HTTP GET Request
      Serial.print("Target:");
      Serial.println(target);
      Serial.print("Life:");
      Serial.println(raisenumber);
      Serial.print("Shot:");
      Serial.println(hittime);
      Serial.print("Down:");
      Serial.println(downtime);
      
                   if (target == 1)
                   {
                    Serial.println("Currently in the loop");
                    for (int i = 0; i < raisenumber; i++)
                    {
                     hitvalue = hittime;
                     while (hitvalue > 0)
                     {
                      sensor = analogRead(visensor);
                      if (sensor > 2000)
                      {
                        hitvalue = hitvalue - 1;
                        Serial.println("HIT");
                        Serial.println(sensor);
                        delay(200);
                      }
                      Serial.println("waiting");
                      Serial.println(sensor);
                      resetCheck++;
                      if (resetCheck == 3580){
                       reset = httpGETRequest(serverNameReset).toInt();
                       Serial.println("ResetCheck");
                       resetCheck = 0; 
                      }
                      if(reset==1){
                        break;
                        }
                     }
                      if(reset==1){
                        break;
                        }                     
                     TargetDown();
                     delay(downtime*1000);
                     TargetUp();
                   } 
      previousMillis = currentMillis;
        }
    } 
    else {
      Serial.println("WiFi Disconnected");
      WiFi.begin(ssid, password);
      Serial.println("Connecting");
      while(WiFi.status() != WL_CONNECTED) { 
      delay(500);
      Serial.print(".");
      }
    }
  }
}
