#include "arduino_secrets.h"
#include "thingProperties.h"  

int lightPin = A0; //analog pin for light sensor - phototransistor 
int tempPin = A1; //analog pin for temperature  
int moisturePin = A2; // analog pin for soil moisture input
int ledPin = 3; //Digital pin for LED  
int moistureThreshold = 600; // analogue value of (dry + wet) / 2 ; The easiest way to calibrate this value is to test the sensor in both dry and wet earth
int lightThreshold = 15; //Recheck during the day this is dependent on whether the plant is a high, medium or low sun plant


void setup() {  
   Serial.begin(9600);  // Initialize serial and wait for port to open:  
   delay(2000); //waiting for Serial Monitor to be ready for output
   
   pinMode(ledPin, OUTPUT);//set LED to be output
  
     
   initProperties();  //Initialize Library- thingProperties.h
   ArduinoCloud.begin(ArduinoIoTPreferredConnection);  //Connect to Arduino Cloud
   /*  
   The following function allows you to obtain more information  
   related to the state of network and IoT Cloud connection and errors  
   the higher number the more granular information you’ll get.  
   The default is 0 (only errors).  
   Maximum is 4  
   */  
   setDebugMessageLevel(2);  
   ArduinoCloud.printDebugInfo();  //debug messages information
}


void loop() {  
 ArduinoCloud.update();  

  message = "";
  light_Events();
  temperature_Events();
  moisture_Events();   
  
  //digitalWrite(ledPin, LOW);
  //LowPower.sleep(60000);//sleep for 30 seconds. You can reset it to sleep for as long as you want to save power
  //message = "Woke up from an interrupt!"; //How to take it out of sleep.
  delay(1000);
} 

void light_Events(){
   light = analogRead(lightPin); //assign light variable to light sensor values 
  
   if (light<lightThreshold){
     message += "Your plants more light! \n";
     digitalWrite(ledPin, HIGH);
   }
   else if (light>1000){
     message +="There is too much sun! \n";
     digitalWrite(ledPin, HIGH);
     }
    else{
      digitalWrite(ledPin, LOW);
    }
}

void temperature_Events(){
   temperature = get_temperature(); /* assign temperature variable to temperature in Fahrenheit */  
   Serial.print("temperature (F): ");  
   Serial.println(temperature); 
}

void moisture_Events(){
  moisture = get_average_moisture();
   /* assign the message variable based on water levels */  
   if (moisture > moistureThreshold) { //high "moisture value" -> high voltage -> low current ->dry plant
     digitalWrite(ledPin, HIGH);
     message += "Warning! Your plant needs water \n"; /* Insert here your emergency message */  
   } 
   else {  
     message += "Your plant has enough water. Great Job!";  
   }
}

int get_average_moisture() {  
   int tempValue = 0; // variable to temporarily store moisture value
   /* make an average of 10 values to be more accurate */  
   for (int a = 0; a < 10; a++) {  
   tempValue += analogRead(moisturePin); 
   delay(100);  
   }  
   return tempValue / 10;  
}  


float get_temperature() {  
 int reading = analogRead(tempPin);  
 float voltage = reading * 3.3;  
 voltage /= 1024.0;  
 /* temperature in Celsius */  
 float temperatureC = (voltage - 0.5) * 100 ; /*converting from 10 mv per degree with 500 mV offset */  
 /* Convert to Fahrenheit */  
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;  
 return temperatureF;  
}  
/*
  Since Message is READ_WRITE variable, onMessageChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onMessageChange()  {
  // Add your code here to act upon Message change
}