//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library
// Settling time (number of samples) and data filtering can be adjusted in the HX711_ADC.h file

#include <HX711_ADC.h>

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell(9, 10);

float val = 0 ;
bool resetFlag = false;
bool dir, prevDir;
//-------------------------------------------------------------------------------------
//Settings
//-------------------------------------------------------------------------------------

#define LOWERTHRESHOLD 10
#define UPPERTHRESHOLD 25
#define DIR_PIN A1
#define TARE_PIN 8
#define TRIGGER_PIN 7
#define SETTLE_TIME 500
#define LED_PIN 6
#define INVERT_Z_DIR false//true  //false if Z_DIR is HIGH w hen moving towards bed.
#define DEBUG 1 //switch serial output

//-------------------------------------------------------------------------------------
// TARE's the loadcell value and resets all outputs
//-------------------------------------------------------------------------------------
void tareLoadCell()
{
  #ifdef LED_PIN
    digitalWrite(LED_PIN,LOW);
  #endif
  digitalWrite(TRIGGER_PIN,LOW);
  delay(SETTLE_TIME);
  LoadCell.begin();
  LoadCell.start(4000);
  LoadCell.setCalFactor(700); // user set calibration factor (float)
  #ifdef DEBUG
    Serial.print(35);
    Serial.print(" ");
  #endif

}

//-------------------------------------------------------------------------------------
// Setup fucntion
//-------------------------------------------------------------------------------------
void setup()
{
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Wait...");
  #endif
  LoadCell.begin();
  LoadCell.start(2000);
  LoadCell.setCalFactor(700.25); // user set calibration factor (float)
  #ifdef DEBUG
  Serial.println("Startup + tare is complete");
  #endif
  pinMode(TRIGGER_PIN,OUTPUT);
  digitalWrite(TRIGGER_PIN,LOW);
  #ifdef LED_PIN
    pinMode(LED_PIN,OUTPUT);
    digitalWrite(LED_PIN,LOW);
  #endif
  pinMode(DIR_PIN,INPUT);
  prevDir = digitalRead(DIR_PIN);

}

//-------------------------------------------------------------------------------------
// Main Loop
//-------------------------------------------------------------------------------------
void loop()
{

      #ifdef DEBUG
        Serial.print(20);
        Serial.print(" ");
      #endif

      LoadCell.update();
      Serial.print(LoadCell.getData());

      val = LoadCell.getData();


      if (val >= LOWERTHRESHOLD && val < UPPERTHRESHOLD)
      {
        digitalWrite(TRIGGER_PIN,HIGH);
        #ifdef LED_PIN
          digitalWrite(LED_PIN,HIGH);
        #endif
        #ifdef DEBUG
          Serial.print(" ");
          Serial.print(LOWERTHRESHOLD);
        #endif
      }

      if (val >= UPPERTHRESHOLD)
        {
          digitalWrite(TRIGGER_PIN,HIGH);
          #ifdef LED_PIN
            digitalWrite(LED_PIN,HIGH);
          #endif
          delay (50);
          digitalWrite(TRIGGER_PIN,LOW);
          #ifdef LED_PIN
            digitalWrite(LED_PIN,LOW);
          #endif
          delay(50);
          #ifdef DEBUG
          Serial.print(" ");
          Serial.print(LOWERTHRESHOLD);
          #endif
        }

      else
        {
          digitalWrite(TRIGGER_PIN,LOW);
          #ifdef LED_PIN
            digitalWrite(LED_PIN,LOW);
          #endif
          #ifdef DEBUG
          Serial.print(" ");
          Serial.print(0);
          #endif
        }
      #ifdef DEBUG
        Serial.println();
      #endif
    }
