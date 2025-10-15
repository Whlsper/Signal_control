#include <Arduino.h>
#include "arduinolib1.h"
#include <string.h>
#include "global1.h"
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>


Pin Pin1(35, 1000, 1000, LOW, LOW);
Pin Pin2(33, 2000, 1000, LOW, LOW);
Pin Pin3(31, 3000, 1000, LOW, LOW);
Pin Pin4(29, 1000, 1000, LOW, LOW);
Pin Pin5(27, 2000, 1000, LOW, LOW);
Pin Pin6(25, 3000, 1000, LOW, LOW);

//Pin array={Pin1,Pin2,Pin3,Pin4,Pin5,Pin6};
LiquidCrystal_I2C lcd(0x27, 20, 4);
unsigned long int start = 0;
unsigned long int start2 = 0;
int prevButtonState = HIGH;
bool First_start = LOW;
bool Timer1_start = LOW;
bool Second_start = LOW;
bool Timer2_start = LOW;
bool flag1 = LOW;
char keys[4][4] = {
  {'1', '2', '3', 'E'},
  {'4', '5', '6', '<'},
  {'7', '8', '9', '>'},
  {'Y', '0', 'N', 'D'}
};
byte rowPins[4] = {39, 41, 43, 45};
byte colPins[4] = {47, 49, 51, 53};
Keypad kp = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);


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

int inputNumber() {
  String c = "";
  lcd.cursor();
  char k = kp.waitForKey();
  if (k == '<') {
    return -2;
  }
  else if (k == '>') {
    return -3;
  }
  else {
    while (k != 'Y') {
      if (k == 'N') {
        c.remove(c.length() - 1);
        lcd.setCursor(0, 3);
        lcd.print(c);
        lcd.noCursor();
        for (int i = c.length(); i < 20; i++) {
          lcd.write('\0');
        }
        lcd.setCursor(c.length(), 3);
        lcd.cursor();
        k = kp.waitForKey();
      }
      else {
        c += k;
        lcd.write(k);
        k = kp.waitForKey();
      }
    }
    lcd.noCursor();
    return c.toInt();
  }
}

void full_pin_setup() {
  lcd.clear();
  for (int i = 0; i < 12; i++) {
Num1:
    if (i < 6) {
      lcd.print("Type OpDel for Pin");
      lcd.print(i + 1);
      lcd.setCursor(0, 1);
      int val;
      EEPROM.get(2 * i, val);
      lcd.print("Prev val:");
      lcd.print(val);
      lcd.setCursor(0, 3);

      int num = inputNumber();
      if (num == -2 && i != 0) {
        i -= 2;
        lcd.clear();
      }
      else if (num == -3) {
        lcd.clear();
        continue;
      }
      else if (num == -2 && i == 0) {
        lcd.clear();
        goto Num1;
      }
      else {
        EEPROM.put(2 * i, num);
        delay(250);
        lcd.clear();
      }
    }
    else {
      lcd.print("Type ClDel for Pin");
      lcd.print(i + -5);
      lcd.setCursor(0, 1);
      int val;
      EEPROM.get(2 * i, val);
      lcd.print("Prev val:");
      lcd.print(val);
      lcd.setCursor(0, 3);
      int num = inputNumber();
      if (num == -2) {
        i -= 2;
        lcd.clear();
      }
      else if (num == -3 && i != 11) {
        lcd.clear();
        continue;
      }
      else if (num == -3 && i == 11) {
        lcd.clear();
        goto Num1;
      }
      else {
        EEPROM.put(2 * i, num);
        delay(250);
        lcd.clear();
      }
    }
  }
}

void direct_pin_setup() {
  start:
  int opval, clval;
  lcd.clear();
  lcd.home();
  lcd.print("Choose Pin (1-6)");
  char c = kp.waitForKey();
  int i = int(c) - 48;
  if(i>0 && i<7){
  lcd.clear();
  lcd.print("Prev Openvalue:");
  EEPROM.get(2 * (i - 1), opval);
  lcd.setCursor(0, 1);
  lcd.print(opval);
  lcd.setCursor(0, 2);
  lcd.print("Next Openvalue:");
  lcd.setCursor(0, 3);
  lcd.cursor();
  clval = inputNumber();
  EEPROM.put(2 * (i - 1), clval);

  lcd.clear();
  lcd.home();
  lcd.print("Prev Closevalue:");
  EEPROM.get(12 + 2 * (i - 1), opval);
  lcd.setCursor(0, 1);
  lcd.print(opval);
  lcd.setCursor(0, 2);
  lcd.print("Next Closevalue:");
  lcd.setCursor(0, 3);
  lcd.cursor();
  clval = inputNumber();
  EEPROM.put(12 + 2 * (i - 1), clval);
  lcd.clear();
  }
  else {
    lcd.clear();
    lcd.home();
    lcd.setCursor(2,1);
    lcd.print("Wrong input 1-6");
    goto start;
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
Setup:
    lcd.clear();
    lcd.home();
    lcd.print("Select setup mode");
    lcd.setCursor(0, 2);
    lcd.print("(1):Total setup");
    lcd.setCursor(0, 3);
    lcd.print("(2):Specific setup");
    char secondc = kp.waitForKey();
    if (secondc == '1') {
Full_setup:
      full_pin_setup();
      delay_init();
      error_check1();
      if (flag1 == HIGH) {
        flag1 = LOW;
        goto Setup;
      }
      else{};
    }
    else if (secondc == '2') {
Direct_setup:
      direct_pin_setup();
    }
    else{
      error1();
      goto Setup;
    }
Error2:
      lcd.home();
      lcd.print("Continue setup?");
      lcd.setCursor(0, 2);
      lcd.print("(Y):Yes");
      lcd.setCursor(0, 3);
      lcd.print("(N):No , operate");
      char c2 = kp.waitForKey();
      if (c2 == 'Y') {
        goto Setup;
      }
      else if (c2 == 'N') {
        lcd.clear();
        delay_init();
        error_check1();
        if (flag1 == HIGH) {
          flag1 = LOW;
          goto Setup;
        }
      }
      else {
        error1();
        goto Error2;
      }
    
    lcd.print("Ready to operate");
  }
  else if (firstc == 'N') {
    lcd.clear();
    lcd.home();
    delay_init();
    lcd.print("Ready to operate");
    error_check1();
    if (flag1 == HIGH) {
      flag1 = LOW;
      goto Setup;
    }
  }
  else {
    error1();
    goto start;
  }
}



void delay_init() {
  EEPROM.get(0, Pin1.Odelaytime);
  EEPROM.get(2, Pin2.Odelaytime);
  EEPROM.get(4, Pin3.Odelaytime);
  EEPROM.get(6, Pin4.Odelaytime);
  EEPROM.get(8, Pin5.Odelaytime);
  EEPROM.get(10, Pin6.Odelaytime);
  EEPROM.get(12, Pin1.Cdelaytime);
  EEPROM.get(14, Pin2.Cdelaytime);
  EEPROM.get(16, Pin3.Cdelaytime);
  EEPROM.get(18, Pin4.Cdelaytime);
  EEPROM.get(20, Pin5.Cdelaytime);
  EEPROM.get(22, Pin6.Cdelaytime);
}

void error1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error! Invalid key");
  lcd.setCursor(3, 2);
  lcd.print("Press any key");
  kp.waitForKey();
  lcd.clear();
}

void error_check1() {
  Pin array[6] = {Pin1, Pin2, Pin3, Pin4, Pin5, Pin6};
  for (int i = 0; i < 6; i++) {
    if (array[i].Odelaytime < 1 || array[i].Cdelaytime < 1) {
      lcd.clear();
      lcd.home();
      lcd.print("Pin ");
      lcd.print(1 + i);
      lcd.print(" error");
      lcd.setCursor(0, 2);
      lcd.print("Press any key");
      kp.waitForKey();
      flag1 = HIGH;
    }
  }
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
