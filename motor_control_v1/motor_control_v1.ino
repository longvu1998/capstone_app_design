#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <iostream>
#include <sstream>
#define dirPin 26
#define stepPin 25
#define visensor 12
//User-defined values
long raisenumber = 0; //Number of steps
long downtime = 0; //Steps / second
long hittime = 0; //hit
char receivedCommand;
//-------------------------------------------------------------------------------
bool newData, runallowed = false; // booleans for new data from serial, and runallowed flag
int i=0,cooldown=0,sensor = 0, hitvalue = 0;
void setup()
{
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    Serial.begin(115200); //define baud rate
    Serial.println("Type rise up time and down time(sec)");
}

void loop()
{
    if (Serial.available() > 0) //if something comes from the computer
    {
        newData = true; //indicate that there is a new data by setting this bool to true
 
        if (newData == true) //we only enter this long switch-case statement if there is a new command from the computer
        {
                raisenumber = Serial.parseInt(); //stand up value
                Serial.println(raisenumber);
                hittime = Serial.parseInt(); //hit value
                Serial.println(hittime);
                downtime = Serial.parseFloat(); //down time
                Serial.println(downtime); 
                for (int i = 0; i < raisenumber; i++)
                {
                     Serial.println("Target Up");
                     TargetUp(); //raise the target
                     hitvalue = hittime;
                     while (hitvalue > 0)
                     {
                      sensor = analogRead(visensor);
                      if (sensor > 150)
                      {
                        hitvalue = hitvalue - 1;
                        Serial.println("HIT");
                        Serial.println(sensor);
                        delay(200);
                      }
                      Serial.println("waiting");
                     }
                     Serial.println("Target Down");
                     TargetDown();
                     delay(downtime*1000);
                }
        }
    }
}

void TargetDown()
{  
    digitalWrite(dirPin, HIGH);
    for (int i = 0; i <= 400; i++)
               {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(100);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(100);
               }
               i++;
}

void TargetUp()
{
    digitalWrite(dirPin, LOW);
    for (int i = 0; i <= 400; i++)
               {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(100);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(100);
               }
               i++;  
}

//void checkSerial() //function for receiving the commands
//{  
//    if (Serial.available() > 0) //if something comes from the computer
//    {
//        receivedCommand = Serial.read(); // pass the value to the receivedCommad variable
//        newData = true; //indicate that there is a new data by setting this bool to true
// 
//        if (newData == true) //we only enter this long switch-case statement if there is a new command from the computer
//        {
//
//                
//            switch (receivedCommand) //we check what is the command
//            {
// 
// 
//            case 'S': // Stops the motor
//               
//                stepper.stop(); //stop motor
//                stepper.disableOutputs(); //disable power
//                Serial.println("Stopped."); //print action
//                runallowed = false; //disable running
//                break;
//               
//            case 'H': //H: Homing
// 
//                runallowed = true;     
//                Serial.println("Homing"); //Print the message
//                GoHome();// Run the function
//                break;
//  
//            default:  
//                raisenumber = Serial.parseFloat(); //stand up value
//                downtime = Serial.parseFloat(); //down time 
//                for (int i = 0; i <= raisenumber; i++)
//                {
//                     RotateAbsolute(); //raise the target
//                     RunTheMotor(); //function to handle the motor
//                     //an if function will be inplement when V-sensor is in
//                     delay(2000);
//                     runallowed = true;
//                     Serial.println("Target Down");
//                     GoHome();
//                     RunTheMotor(); //function to handle the motor
//                     delay(downtime*1000); 
//                }
//                break;
//            }
//        }
//        //after we went through the above tasks, newData is set to false again, so we are ready to receive new commands again.
//        newData = false;       
//    }
//}
