#include <Arduino.h>
#include "arduinolib1.h" 
#include <string.h>
#include "global1.h"

Pin Pin1(8,1000,1000,LOW,LOW);
Pin Pin2(9,2000,1000,LOW,LOW);
Pin Pin3(10,3000,1000,LOW,LOW);
Pin Pin4(3,1000,1000,LOW,LOW);
Pin Pin5(4,2000,1000,LOW,LOW);
Pin Pin6(5,3000,1000,LOW,LOW);
unsigned long int start=0;
unsigned long int start2=0;
int prevButtonState=HIGH;
bool First_start=LOW;
bool Timer1_start=LOW;
bool Second_start=LOW;
bool Timer2_start=LOW;



Pin::Pin(int pn,int odt,int cdt,bool ho,bool hc){
  pinNumber=pn;
  Odelaytime=odt;
  Cdelaytime=cdt;
  hasOpened=ho;
  hasClosed=hc;
  pinMode(pinNumber,OUTPUT);
}

void Pin::activate(unsigned long int start){
  if(millis()-start>=Odelaytime && !hasOpened){
    digitalWrite(pinNumber,HIGH);
    Serial.println(millis()-start);
    hasOpened=HIGH;
  }
  if(millis()-start>=Odelaytime+Cdelaytime && !hasClosed){
    digitalWrite(pinNumber,LOW);
    Serial.println(millis()-start);
    hasClosed=HIGH;
  }
}

void check1(){
  if(First_start && Timer1_start && Pin1.hasOpened && Pin2.hasOpened && Pin3.hasOpened && Pin1.hasClosed && Pin2.hasClosed && Pin3.hasClosed){
      First_start=LOW; Timer1_start=LOW;Pin1.hasOpened=LOW;Pin2.hasOpened=LOW;Pin3.hasOpened=LOW;Pin1.hasClosed=LOW;Pin2.hasClosed=LOW;Pin3.hasClosed=LOW;
    }
}

void check2(){
  if(Second_start && Timer2_start && Pin4.hasOpened && Pin5.hasOpened && Pin6.hasOpened && Pin4.hasClosed && Pin5.hasClosed && Pin6.hasClosed){
      Second_start=LOW; Timer2_start=LOW;Pin4.hasOpened=LOW;Pin5.hasOpened=LOW;Pin6.hasOpened=LOW;Pin4.hasClosed=LOW;Pin5.hasClosed=LOW;Pin6.hasClosed=LOW;
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
