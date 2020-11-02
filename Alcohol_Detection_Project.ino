/*==========================================================================
 * Project: Alcohol Detection System
 * Author:  Gyan Prakash
 * Date created: September 2018
 * Arduino IDE version: 1.8.5
 * Operating System: Windows 10 Pro
 * 
 * ***WARNING*** : I dont appreciate use this as a real life breathalyser. 
 * 
 * Description: 
 * Its a part of my 3rd semester Measurement lab project.
 * The Alcohol detection project aims to roughly measure the concentration of alcohol in the air. 
 * The values obtained can be trusted for breathtesting purposes. 
 * I am not responsible for any actions that are taken on the basis of my observations from the use of this code (or part thereof)
 * 
 * The Alcohol sensor reacts to the presence of these substances in the air
 *   - Alcohol (ethanol)
 *   - Isopropyl alcohol
 *   - Butane
 *   - LPG gas
 *   - Petrol fumes
 *   - possibly other substances that I have not yet tested.
 * Therefore a noticeable sensor reaction can not assumed to be from the presence of Alcohol (ethanol) in the air.
 *   
 * Acknowledgements:
 * The idea is adapted from the Seeedstudio wiki code:
 * http://wiki.seeedstudio.com/Grove-Alcohol_Sensor/           
 ============================================================================ */



/*=======================================================================================
 * Libraries:
 * Wire.h = used for I2C communication - which is used by the Grove LCD screen
 * rgb_lcd.h = simplifies the operation of the LCD screen
 * math.h = required for the pow() function which is used to calculate the concentration of alcohol in the air.
 ========================================================================================*/
#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>  



/*=======================================================================================
 * GLOBAL VARIABLES:
 * lcd = used to control the lcd text messages
 * point 1,2 and 3 = are variables used to hold the sensor readings
 * ratio = holds the ratio of alcoholic air to clean air
 * alc = holds the concentraton of alcohol in the air in parts per million (ppm)
 * titleCounter = is a variable which aids in the clean up of residual text on the lcd screen
 ========================================================================================*/
rgb_lcd lcd;
float point1;
float point2;
float point3;
float ratio;
double alc;
int titleCounter=0;



/*=======================================================================================
 * SETUP:
 * Initialise the LCD (16 columns, 2 rows)
 * Create introductory text for the project
 * Set Analog Pin 1 as an OUTPUT pin, and set it to LOW. This activates the heater on the Alcohol sensor, 
 *   which sometimes causes the program to hang. If it hangs, just reset the Arduino.
 * If the program successfully passes the "heater activation" section, we want to display a confirmation message.
 * Then prepare the LCD for the next section (loop).
 ========================================================================================*/
void setup() {
  lcd.begin(16,2);
  
  printToLCD("Alcohol Project","",3000);
  printToLCD("Alcohol Project","by ARDUINOBASICS",3000);
  lcd.clear();
  
  pinMode(A1,OUTPUT);                                    // heater pin is on Analog Pin (A1)
  digitalWrite(A1,LOW);
  printToLCD("Heater", "Activated", 3000);
  printToLCD("Reading:", "", 10);
}




/*=======================================================================================
 * LOOP:
 * Take a reading from the Alcohol sensor and assign it to variable "point1"
 * Print the reading on the LCD screen
 * 
 * Take another reading from the sensor and assign it to "point2"
 * If the point2 reading is LOWER than point1, then take a third reading.
 * If the point3 reading is LOWER than point2, then we will do the Alcohol concentration calculations
 * 
 * Calculate the ratio of point3 to point1
 * Use the ratio to calculate the alcohol concentration in the air.
 * The alc formula was derived from extrapolating the points from the datasheet and plotting them in Microsoft Excel
 * Once plotted in Excel, I obtained the curve calulation and imported it back into this program.
 * 
 * WARNING ************************************************************
 * Please note, that this calculation does NOT produce accurate results.  It is for demonstration purposes only - and a proof of concept. 
 * This should not be used for making any decisions in real life. 
 * The calculated Alcohol in parts per million (ppm) is printed to the LCD.
 * 
 * The titleCounter is used to tidy up stray characters that are left behind from bigger numbers.
 * This is because I do not clear the screen every time. I don't like the flickering. 
 * So I clear the screen after every 20 readings.
 ========================================================================================*/
void loop() {
  point1 = readSensor();
  lcd.setCursor(0,1);
  lcd.print(point1);
  delay(100);
  
  point2 = readSensor();
  if(point2<point1){
    delay(100);
    
    point3 = readSensor();
    if(point3<point2){
      printToLCD("Alcohol (ppm):","",10);
    
      while(point3<point2){
        ratio = point3 / point1;
        point3 = readSensor();
        alc = 3.9839 * pow(ratio, -2.188);
        lcd.setCursor(0,1);
        lcd.print(alc);
        delay(100);
        
        titleCounter++;
        if(titleCounter>20){
            printToLCD("Alcohol (ppm):","",10);
            titleCounter=0;
        }
      }
      printToLCD("Reading:","",10);
    }
  }
}


/*=======================================================================================
 * readSensor():
 * Take the avareage of 100 readings from Analog pin 0.
 * Calculate the voltage from the analog reading, and assign it to the sensorVolt variable.
 *  - for greater accuracy, replace Vref with your own reference voltage measurement
 * Calculate the resistance using a voltage divider equation, and assign it to RS_gas.
 *  - the 10k resistor (R16 on the alcohol sensor datasheet) is not included in the
 *    calculation because it is later cancelled out in the "ratio" calculation.
 * The resistance of the gas being measured is returned.
 ======================================================================================= */
float readSensor(){
  float vRef = 5.0
  float sensorValue = 0;
  float sensorVolt;
  float RS_gas;

  for(int x = 0 ; x < 100 ; x++){
    sensorValue = sensorValue + analogRead(A0);
  }
  sensorValue = sensorValue/100.0;
  sensorVolt = (float)(sensorValue*vRef)/1024;
  RS_gas = sensorVolt/(vRef-sensorVolt);
  return RS_gas;
}


/*=======================================================================================
 * printToLCD():
 * a function that aims to further simplify the process of writing to the LCD screen.
 ======================================================================================= */
void printToLCD(String line1, String line2, int timeDelay){
  lcd.clear();
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
  delay(timeDelay);
}
