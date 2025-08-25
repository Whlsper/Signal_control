#ifndef arduinolib3_h
#define arduinolib3_h
#include <Arduino.h>
#include <string.h>

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




#endif
