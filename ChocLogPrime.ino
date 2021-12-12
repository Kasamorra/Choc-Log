//Choc Log Prime: Temperature Logger with Choc Mode and Custom Mode
//An alarm goes off when the temperature threshold is reached
//Components: DHT11, LCD, Active Buzzer, Joystick
//Pins DHT11: 2 
//Pins LCD: 7, 8, 9, 10, 11, 12
//Pins Buzzer: 5
//Pins Joystick: 3, A0

#include <DHT.h> //library for the HT sensor
#include <DHT_U.h> //library for the HT sensor 
#include<LiquidCrystal.h> //library for the LCD

//HT-Sensor DHT11
#define Type DHT11
const int sensePin = 2;
DHT HT(sensePin, Type);
const int sTime = 500; //set-up time for temp sens

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

//Joystick
const int yPin = A0
const int switchPin = 3;
int yVal; //y-value of joystick
const int valUp = 300; //guideline value for up-direction of joystick
const int valDown = 800; //guideline value for down-direction of joystick
int switchValOld = 1; //old state (0 = pressed, 1 = not pressed)
int switchValNew; //new state

//Other
const float chocTemp = 28.00; //melting point of chocolate and threshold of choco mode
float customTemp = 18.00; //custom threshold
const float unitStep = 0.10; //value by which custom threshold is to be increased/decreased
float tempC; //current temperature
float tempOld = 50; //setting it to an unreachable value, just for the start
int menuMode = 0; //0 = home menu, 1 = choc mode, 2 = setting temp, 3 = custom mode
int arrowSelect = 0; //0 = Choc Mode, 1 = Custom Mode
const int delayTime = 300;

void checkTemp(float thresholdTemp); //function that checks the current temperature and compares it to the threshold


void setup() {

  //HT-Sensor
  HT.begin();
  delay(sTime); //buffer time for the sensor to set things up

  //LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); //sets position of cursor to top left corner
  lcd.print("Choc Log"); //logo when turning device on
  delay(3500);

  //Buzzer
  pinMode(buzzPin, OUTPUT);

  //Joystick
  pinMode(yPin, INPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);
}

void loop() {
  //Initial reset
  arrowSelect = 0;

  //Switching between different screens
  switch (menuMode)
  {
    //Home Screen / Menu
    case 0:
      digitalWrite(buzzPin, LOW); //turns alarm off, in case it was on
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">Chocolate");
      lcd.setCursor(0, 1);
      lcd.print(" Custom");

      while (menuMode == 0)//selecting one of two modes
      {
        yVal = analogRead(yPin);

        if (yVal <= valUp) //select choco mode
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Chocolate");
          lcd.setCursor(0, 1);
          lcd.print(" Custom");
          arrowSelect = 0; //indicates which mose is currently selected, relevant when joystick is pressed
        }
        else if (yVal >= valDown) //select custom mode
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Chocolate");
          lcd.setCursor(0, 1);
          lcd.print(">Custom");
          arrowSelect = 1; //indicates which mose is currently selected, relevant when joystick is pressed
        }

        //checking if joystick pressed
        switchValNew = digitalRead(switchPin);
        if (switchValOld == 0 && switchValNew == 1)
        {
          if (arrowSelect == 0) //select choco mode
            menuMode = 1;
          else if (arrowSelect == 1) //select custom mode
            menuMode = 2; 
        }
        switchValOld = switchValNew;
        
        delay(delayTime);
      } //bracket belongs to while
      break;

    //Chocolate Mode
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ChocMode: ");
      lcd.print(chocTemp);
      lcd.setCursor(0, 1);

      while (menuMode == 1)
      {
        checkTemp(chocTemp); //checks current temp and compares it with choco threshold

        //checks if joystick is pressed
        switchValNew = digitalRead(switchPin);
        if (switchValOld == 0 && switchValNew == 1)
          menuMode = 0; //-> back to menu
          
        switchValOld = switchValNew;
      } //bracket belongs to while
      break;

    //Setting custom temperature threshold
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set Temperature:");

      while (menuMode == 2)
      {
        lcd.setCursor(0, 1);
        lcd.print(customTemp);

        yVal = analogRead(yPin);
        
        //increasing threshold
        if (yVal <= valUp)
          customTemp = customTemp + unitStep;

        //decreasing threshold
        else if (yVal >= valDown)
          customTemp = customTemp - unitStep;

        //checking if joystick is pressed
        switchValNew = digitalRead(switchPin);
        if (switchValOld == 0 && switchValNew == 1)
          menuMode = 3; //-> save threshold and start Custom Mode
          
        switchValOld = switchValNew;
        
        delay(delayTime);
      } //bracket belongs to while
      break;

    //Custom Mode
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CustomMode:");
      lcd.print(customTemp);
      lcd.setCursor(0, 1);

      while (menuMode == 3)
      {
        checkTemp(customTemp); //checks current temp and compares it with set threshold

        //checking if joystick is pressed
        switchValNew = digitalRead(switchPin);
        if (switchValOld == 0 && switchValNew == 1)
          menuMode = 0; //-> back to menu
          
        switchValOld = switchValNew;
      } //bracket belongs to while
      break;
      
  } //bracket belongs to switch
} //bracket belongs to void loop


//function reading the current temperature and comparing it to the current threshold
void checkTemp(float thresholdTemp)
{
  tempC = HT.readTemperature(); //reads temperature from sensor

  //checks if temperature has changed, if so, updates screen
  if (tempC != tempOld)
  {
    lcd.setCursor(0, 1);
    lcd.print(tempC);
    lcd.print(" C");
  }

  //comparing current temp to threshold and controlling alarm
  if (tempC >= thresholdTemp) 
    digitalWrite(buzzPin, HIGH);
  else
    digitalWrite(buzzPin, LOW);

  tempOld = tempC;

  delay(delayTime);
}
