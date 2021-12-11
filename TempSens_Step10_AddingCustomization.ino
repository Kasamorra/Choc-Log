//so we have our temp sense connected to LCD
//it shows the current temperature
//once it reaches a certain temperature, the buzzer starts to beep
//now we're trying to add a menu
//Pins used: 2, 3, 5, 7, 8, 9, 10, 11, 12, A0, A3

#include <DHT.h> //library for the HT sensor
#include <DHT_U.h> //library for the HT sensor 
#include<LiquidCrystal.h> //library for the LCD

//HT-Sensor
#define Type DHT11
int sensePin = 2;
DHT HT(sensePin, Type);
int sTime = 500; //delay to set things  up
int delayTime = 700;

//LCD
int rs = 7;
int en = 8;
int d4 = 9;
int d5 = 10;
int d6 = 11;
int d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Buzzer
int buzzPin = 5;

//Joystick
int yPin = A0;
int xPin = A3;
int switchPin = 3;
int xVal;
int yVal;
int valUp = 300;
int valDown = 800;
int switchValOld = 1; //old state (0 = pressed, 1 = not pressed)
int switchValNew; //new state

//Other
float chocTemp = 28; //melting point of chocolate
float customTemp = 18; //custom threshold
float unitStep = 0.1;
float tempC; //current temperature
float tempOld = 50; //setting it to an unreachable value, just for the start
int menuMode = 0; //0 = home menu, 1 = choc mode, 2 = setting temp, 3 = custom mode
int arrowSelect = 0; //0 = Choc Mode, 1 = Custom Mode
const int delayTimeShort = 300;

void checkTemp(float thresholdTemp);


void setup() {

  //for troubleshooting
  Serial.begin(9600);

  //HT-Sensor
  HT.begin();
  delay(sTime); //just some buffer time for the sensor

  //LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); //sets position of cursor to top left corner
  lcd.print("Choc Log");
  delay(3500);
  lcd.setCursor(0, 0);
  lcd.print(">Chocolate");
  lcd.setCursor(0, 1);
  lcd.print(" Custom");

  //Buzzer
  pinMode(buzzPin, OUTPUT);

  //Joystick
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);
}

void loop() {
  arrowSelect = 0;

  switch (menuMode)
  {
    case 0: //home screen
      digitalWrite(buzzPin, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">Chocolate");
      lcd.setCursor(0, 1);
      lcd.print(" Custom");

      while (menuMode == 0)
      {
        xVal = analogRead(xPin);
        yVal = analogRead(yPin);

        if (yVal <= valUp)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Chocolate");
          lcd.setCursor(0, 1);
          lcd.print(" Custom");
          arrowSelect = 0;
        }
        else if (yVal >= valDown)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Chocolate");
          lcd.setCursor(0, 1);
          lcd.print(">Custom");
          arrowSelect = 1;
        }

        //checking if button pressed
        switchValNew = digitalRead(switchPin);
        if (switchValOld == 0 && switchValNew == 1)
        {
          if (arrowSelect == 0)
            menuMode = 1;
          else if (arrowSelect == 1)
            menuMode = 2;
        }
        switchValOld = switchValNew;
        delay(delayTimeShort);
      } //while
      break;

    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ChocMode: ");
      lcd.print(chocTemp);
      lcd.setCursor(0, 1);

      while (menuMode == 1)
      {
        checkTemp(chocTemp);

        switchValNew = digitalRead(switchPin);

        if (switchValOld == 0 && switchValNew == 1)
          menuMode = 0;

        switchValOld = switchValNew;
      } //while
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Temperature:");

      while (menuMode == 2)
      {
        lcd.setCursor(0, 1);
        lcd.print(customTemp);

        xVal = analogRead(xPin);
        yVal = analogRead(yPin);

        if (yVal <= valUp)
          customTemp = customTemp + unitStep;

        else if (yVal >= valDown)
          customTemp = customTemp - unitStep;

        switchValNew = digitalRead(switchPin);

        if (switchValOld == 0 && switchValNew == 1)
          menuMode = 3;

        switchValOld = switchValNew;
        delay(delayTimeShort);
      } //while
      break;

    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CustomMode:");
      lcd.print(customTemp);
      lcd.setCursor(0, 1);

      while (menuMode == 3)
      {
        checkTemp(customTemp);

        switchValNew = digitalRead(switchPin);

        if (switchValOld == 0 && switchValNew == 1)
          menuMode = 0;

        switchValOld = switchValNew;
      }
      break;
  }
}

void checkTemp(float thresholdTemp)
{
  tempC = HT.readTemperature(); //sensor measures temperature

  if (tempC != tempOld)
  {
    lcd.setCursor(0, 1); //sets position of cursor to bottom left corner
    lcd.print(tempC);
    lcd.print(" C");
  }

  if (tempC >= thresholdTemp)
    digitalWrite(buzzPin, HIGH);
  else
    digitalWrite(buzzPin, LOW);

  tempOld = tempC;

  delay(delayTime);
}
