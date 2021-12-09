//Temperature Logger that starts an alarm when certain temperature is detected
//Components: HT-Sensor, LCD, Buzzer
//Pins used: 2, 5, 7, 8, 9, 10, 11, 12

#include <DHT.h> //library for the HT sensor
#include <DHT_U.h> //library for the HT sensor 
#include<LiquidCrystal.h> //library for the LCD

//HT-Sensor
#define Type DHT11
const int sensePin = 2;
DHT HT(sensePin, Type);
const int sTime = 500; //delay for the sensor to set things  up

//LCD
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Buzzer
const int buzzPin = 5;

//General
const int delayTime = 700;
const float chocTemp = 28; //melting point of choc, threshold for alarm
float tempC; //current temperature
float tempOld = 50; //high value only for initialisation

void setup() {

  //Troubleshooting
  Serial.begin(9600);

  //HT-Sensor
  HT.begin();
  delay(sTime); //buffer time for the sensor

  //LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); //sets position of cursor to top left corner
  lcd.print("Choc Log");

  //Buzzer
  pinMode(buzzPin, OUTPUT);
}

void loop() {

  tempC = HT.readTemperature(); //sensor measures current temperature

  /* FOR TROUBLESHOOTING ONLY
  Serial.print("C: ");
  Serial.print(tempC);
  Serial.print(", Old: ");
  Serial.println(tempOld);
  delay(delayTime); */

  //Updating the display
  if (tempC != tempOld) //checks if there is a change in temperature
  {
    lcd.setCursor(0, 1); //sets position of cursor to bottom left corner
    lcd.print(tempC);
    lcd.print(" C");
  }

  //Controlling the alarm
  if (tempC >= chocTemp)
    digitalWrite(buzzPin, HIGH);
  else
    digitalWrite(buzzPin, LOW);

  tempOld = tempC; //Updating "old" temperature

}
