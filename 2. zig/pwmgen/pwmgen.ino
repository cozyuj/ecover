/*
 * pwm transmitter
 * Created by shine@paran.com at 2020.12.01
 * 
 */

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define   m1  5       // motor
#define   m2  6
#define   m3  9
#define   m4  10

#define   sw1 8       // switch - motor select
#define   sw2 7
#define   sw3 4
#define   sw4 3

#define   vr  A0      // VR -- change pwm value

int val_1=0, val_2=0, val_3=0, val_4=0;     // pwm value for motors
  
void setup()
{
  Serial.begin(9600);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
  pinMode(sw4, INPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("___ECOVER___");
}

void loop()
{ 
  if(sw1 == HIGH){
    val_1 = analogRead(vr);
    analogWrite(m1, val_1);
  }else{
    analogWrite(m1, val_1);
  }
  if(sw2 == HIGH){
    val_2 = analogRead(vr);
    analogWrite(m2, val_2);
  }else{
    analogWrite(m1, val_2);
  }
  if(sw3 == HIGH){
    val_3 = analogRead(vr);
    analogWrite(m3, val_3);
  }else{
    analogWrite(m1, val_3);
  }
  if(sw4 == HIGH){
    val_4 = analogRead(vr);
    analogWrite(m4, val_4);
  }else{
    analogWrite(m1, val_4);
  }
  
  lcd.setCursor(0, 1);
  lcd.print(val_1);
  lcd.setCursor(0, 8);
  lcd.print(val_2);
  lcd.setCursor(1, 1);
  lcd.print(val_3);
  lcd.setCursor(1, 8);
  lcd.print(val_4);
}
