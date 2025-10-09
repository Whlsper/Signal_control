#include <Arduino.h>
#include "arduinolib1.h"
#include <string.h>
#include "global1.h"
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>


Pin Pin1(0, 1000, 1000, LOW, LOW);
Pin Pin2(1, 2000, 1000, LOW, LOW);
Pin Pin3(2, 3000, 1000, LOW, LOW);
Pin Pin4(3, 1000, 1000, LOW, LOW);
Pin Pin5(4, 2000, 1000, LOW, LOW);
Pin Pin6(5, 3000, 1000, LOW, LOW);

//Pin array={Pin1,Pin2,Pin3,Pin4,Pin5,Pin6};
LiquidCrystal_I2C lcd(0x27, 20, 4);
unsigned long int start = 0;
unsigned long int start2 = 0;
int prevButtonState = HIGH;
bool First_start = LOW;
bool Timer1_start = LOW;
bool Second_start = LOW;
bool Timer2_start = LOW;
char keys[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'Y', '0', 'N'}
};
byte rowPins[4] = {12, 11, 10, 9};
byte colPins[3] = {8, 7, 6};
Keypad kp = Keypad(makeKeymap(keys), rowPins, colPins, 4, 3);


Pin::Pin(int pn, int odt, int cdt, bool ho, bool hc) {
  pinNumber = pn;
  Odelaytime = odt;
  Cdelaytime = cdt;
  hasOpened = ho;
  hasClosed = hc;
  pinMode(pinNumber, OUTPUT);
}

void Pin::activate(unsigned long int start) {
  if (millis() - start >= Odelaytime && !hasOpened) {
    digitalWrite(pinNumber, HIGH);

    hasOpened = HIGH;
  }
  if (millis() - start >= Odelaytime + Cdelaytime && !hasClosed) {
    digitalWrite(pinNumber, LOW);

    hasClosed = HIGH;
  }
}

void check1() {
  if (First_start && Timer1_start && Pin1.hasOpened && Pin2.hasOpened && Pin3.hasOpened && Pin1.hasClosed && Pin2.hasClosed && Pin3.hasClosed) {
    First_start = LOW; Timer1_start = LOW; Pin1.hasOpened = LOW; Pin2.hasOpened = LOW; Pin3.hasOpened = LOW; Pin1.hasClosed = LOW; Pin2.hasClosed = LOW; Pin3.hasClosed = LOW;
  }
}

void check2() {
  if (Second_start && Timer2_start && Pin4.hasOpened && Pin5.hasOpened && Pin6.hasOpened && Pin4.hasClosed && Pin5.hasClosed && Pin6.hasClosed) {
    Second_start = LOW; Timer2_start = LOW; Pin4.hasOpened = LOW; Pin5.hasOpened = LOW; Pin6.hasOpened = LOW; Pin4.hasClosed = LOW; Pin5.hasClosed = LOW; Pin6.hasClosed = LOW;
  }
}

void UI_init() {
  lcd.init();
  lcd.backlight();
  start:
  lcd.setCursor(0, 0);
  lcd.print("Welcome begin setup?");
  lcd.setCursor(0, 2);
  lcd.print("(Y):Yes enter setup");
  lcd.setCursor(0, 3);
  lcd.print("(N):No continue");
  char firstc = kp.waitForKey();
  if (firstc == 'Y') {
    lcd.clear();
    lcd.home();
    beginSetup();
    pin_setup();
    lcd.print("Ready to operate");
  }
  else if (firstc == 'N') {
    lcd.clear();
    lcd.home();
    pin_setup();
    lcd.print("Ready to operate");
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error! Invalid key");
    lcd.setCursor(3,2);
    lcd.print("Press any key");
    kp.waitForKey();
    lcd.clear();
    goto start;
  }
}

int inputNumber() {
  String c = "";
  lcd.cursor();
  char k = kp.waitForKey();
  while (k != 'Y') {
    if(k=='N'){
      c.remove(c.length()-1);
      lcd.setCursor(0,2);
      lcd.print(c);
      lcd.noCursor();
      for(int i=c.length();i<21;i++){
        lcd.write('\0');
      }
      lcd.setCursor(c.length(),2);
      lcd.cursor();
      k=kp.waitForKey();
    }
    else{
      c += k;
      lcd.write(k);
      k = kp.waitForKey();
    }
  }
  lcd.noCursor();
  return c.toInt();
}

void beginSetup(){
  for(int i=0;i<6;i++){
    lcd.print("Type opeDel for Pin");
    lcd.print(i+1);
    lcd.setCursor(0,2);
    int num=inputNumber();
    EEPROM.put(2*i,num);
    delay(250);
    lcd.clear();
  }
  for(int i=0;i<6;i++){
    lcd.print("Type clDel for Pin");
    lcd.print(i+1);
    lcd.setCursor(0,2);
    int num=inputNumber();
    EEPROM.put(12+i*2,num);
    delay(250);
    lcd.clear();
  }

}

void pin_setup(){
  EEPROM.get(0,Pin1.Odelaytime);
  EEPROM.get(2,Pin2.Odelaytime);
  EEPROM.get(4,Pin3.Odelaytime);
  EEPROM.get(6,Pin4.Odelaytime);
  EEPROM.get(8,Pin5.Odelaytime);
  EEPROM.get(10,Pin6.Odelaytime);
  EEPROM.get(12,Pin1.Cdelaytime);
  EEPROM.get(14,Pin2.Cdelaytime);
  EEPROM.get(16,Pin3.Cdelaytime);
  EEPROM.get(18,Pin4.Cdelaytime);
  EEPROM.get(20,Pin5.Cdelaytime);
  EEPROM.get(22,Pin6.Cdelaytime);
}


/*int Receive_serial_info(String s) {
  Serial.println(s);
  String input = "";
  while (input.length() == 0) {
    if (Serial.available()) {
      input = Serial.readStringUntil('\n');
      input.trim();
    }
  }
  return input.toInt();
  }
*/
