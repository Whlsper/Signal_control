#ifndef arduinolib3_h
#define arduinolib3_h
#include <Arduino.h>
#include <string.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>

class Pin{
public:
    int pinNumber;
    int Odelaytime;
    int Cdelaytime;
    bool hasOpened;
    bool hasClosed;
    Pin(int pn,int odt,int cdt,bool ho,bool hc);
    void activate(unsigned long int start);
};



//int Receive_serial_info(String s);
void check1();
void check2();
void UI_init();
int inputNumber();
void full_pin_setup();
void direct_pin_setup();
void delay_init();
void error1();
void error_check1();




#endif
