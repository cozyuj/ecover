#include <LiquidCrystal_I2C.h>

/*
   Tachometer
   Created by shine@paran.com at 2019.12.28
   modified at 2020.03.24 - calculate one cycle
*/

#include <HardwareSerial.h>
//#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

volatile unsigned long rev1 = 0;
int rpm1;
int rpm1_a=0;
int rpm1_b=0;

volatile unsigned long cur1;
volatile unsigned long pre1;
volatile unsigned long cycle1;

volatile unsigned long rev2 = 0;
int rpm2;
volatile unsigned long cur2;
volatile unsigned long pre2;
volatile unsigned long cycle2;

volatile unsigned long rev3 = 0;
int rpm3;
volatile unsigned long cur3;
volatile unsigned long pre3;
volatile unsigned long cycle3;

volatile unsigned long rev4 = 0;
int rpm4;
volatile unsigned long cur4;
volatile unsigned long pre4;
volatile unsigned long cycle4;

int ir_sensor1 = 2;
int ir_sensor2 = 4;
int ir_sensor3 = 18;
int ir_sensor4 = 19;

void isr1() //interrupt service routine
{
  rev1++;

  cur1 = micros();     // current time
  //Serial.print("cur = ");Serial.println(cur);
  if ((cur1 - pre1) > 1) { // count once
    cycle1 = cur1 - pre1;  // time for one turn
    pre1 = cur1;          // copy current time to previous time
  }
}
void isr2() //interrupt service routine
{
  rev2++;

  cur2 = micros();     // current time
  //Serial.print("cur = ");Serial.println(cur);
  if ((cur2 - pre2) > 1) { // count once
    cycle2 = cur2 - pre2;  // time for one turn
    pre2 = cur2;          // copy current time to previous time
  }
}
void isr3() //interrupt service routine
{
  rev3++;

  cur3 = micros();     // current time
  //Serial.print("cur = ");Serial.println(cur);
  if ((cur3 - pre3) > 1) { // count once
    cycle3 = cur3 - pre3;  // time for one turn
    pre3 = cur3;          // copy current time to previous time
  }
}
void isr4() //interrupt service routine
{
  rev4++;

  cur4 = micros();     // current time
  //Serial.print("cur = ");Serial.println(cur);
  if ((cur4 - pre4) > 1) { // count once
    cycle4 = cur4 - pre4;  // time for one turn
    pre4 = cur4;          // copy current time to previous time
  }
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(ir_sensor1, INPUT);
  pinMode(ir_sensor2, INPUT);
  pinMode(ir_sensor3, INPUT);
  pinMode(ir_sensor4, INPUT);
  pre1 = micros();  //시작 시간
  pre2 = micros();  //시작 시간
  pre3 = micros();  //시작 시간
  pre4 = micros();  //시작 시간

  attachInterrupt(digitalPinToInterrupt(ir_sensor1), isr1, RISING);  //attaching the interrupt
  attachInterrupt(digitalPinToInterrupt(ir_sensor2), isr2, RISING);  //attaching the interrupt
  attachInterrupt(digitalPinToInterrupt(ir_sensor3), isr3, RISING);  //attaching the interrupt
  attachInterrupt(digitalPinToInterrupt(ir_sensor4), isr4, RISING);  //attaching the interrupt

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("___ECOVER___");
}

void loop()
{
  delay(1000);

  Serial.print("cycle1 = "); Serial.println(cycle1);
  Serial.print("rev1 = "); Serial.println(rev1);
  Serial.print("cycle1 = "); Serial.println(cycle1);
  Serial.print("rpm1 = "); Serial.println(60000000 / cycle1);

  rpm1 = 60000000 / cycle1;     //calculates rpm
  rpm1_a = rpm1/100;
  rpm1_b = rpm1%100;
  Serial.println(rpm1);
  Serial2.write(byte(rpm1_a));
  Serial2.write(byte(rpm1_b));

  Serial.print("cycle2 = "); Serial.println(cycle2);
  Serial.print("rev2 = "); Serial.println(rev2);
  Serial.print("cycle2 = "); Serial.println(cycle2);
  Serial.print("rpm2 = "); Serial.println(60000000 / cycle2);

  rpm2 = 60000000 / cycle2;     //calculates rpm
  Serial.println(rpm2);
  //Serial2.write(rpm2);

  Serial.print("cycle3 = "); Serial.println(cycle3);
  Serial.print("rev3 = "); Serial.println(rev3);
  Serial.print("cycle3 = "); Serial.println(cycle3);
  Serial.print("rpm3 = "); Serial.println(60000000 / cycle3);

  rpm3 = 60000000 / cycle3;     //calculates rpm
  Serial.println(rpm3);
  //Serial2.write(rpm3);

  Serial.print("cycle4 = "); Serial.println(cycle4);
  Serial.print("rev4 = "); Serial.println(rev4);
  Serial.print("cycle4 = "); Serial.println(cycle4);
  Serial.print("rpm4 = "); Serial.println(60000000 / cycle4);

  rpm4 = 60000000 / cycle4;     //calculates rpm
  Serial.println(rpm4);
  //Serial2.write(rpm4);

  lcd.setCursor(0, 1);
  lcd.print(rpm1);
  lcd.setCursor(0, 8);
  lcd.print(rpm2);
  lcd.setCursor(1, 1);
  lcd.print(rpm3);
  lcd.setCursor(1, 8);
  lcd.print(rpm4);  
}
