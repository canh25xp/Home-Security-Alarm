int detectPin= 13;
int alarmPin = 2;
int buttonPin= 3;
#include "pitches.h"
#include "sound.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

unsigned long runTime=0;
unsigned long lastReceivedTime=0;
unsigned long lastDebounceTime=0;
unsigned long holdTime=0;
unsigned long debounceDelay=250;
unsigned long minHoldTime=5000;
unsigned long autoTurnoffTime=20000;

boolean lastState=0;
boolean state=0;
boolean actualState=0;
boolean button=0;

unsigned int count=0;

void setup(){
  Serial.begin(9600);
  pinMode(detectPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello!");
  nokia();
  lcd.clear();
  Serial.println("Alarm ready.");  
  lcd.print("Alarm ready.");
}
void loop(){
  runTime=millis();
  state = digitalRead(detectPin);
  button=digitalRead(buttonPin);
  if(state!=lastState){
    lastDebounceTime=runTime;
  }
  holdTime = (runTime - lastDebounceTime);
  if(holdTime > debounceDelay){
    actualState=state;
  }
  lastState = state;
  if(actualState==1 && holdTime < minHoldTime){
    lastReceivedTime=runTime;
    count++;
    lcd.backlight();
    lcd.clear();
    lcd.print("State = ");    lcd.print(actualState);
    lcd.setCursor(0,1);
    lcd.print("Count = ");    lcd.print(count);
    Serial.print("State = ");
    Serial.println(actualState);
    alarmSound();
    actualState=0;
    delay(5000);
  }
  if(button==0){
    lastReceivedTime=runTime;
    lcd.backlight();          
    lcd.clear();
    lcd.print("State = ");    lcd.print(actualState);
    lcd.setCursor(0,1);
    lcd.print("Count = ");    lcd.print(count);
    delay(500);
  }
  if((runTime - lastReceivedTime) > autoTurnoffTime){
    lcd.clear();
    lcd.noBacklight();
  }
}
