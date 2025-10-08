#include "arduinolib1.h"
#include "global1.h"

void setup(){
Serial.begin(9600);
pinMode(2,INPUT_PULLUP);

}

void loop(){
 int currentButtonState = digitalRead(2);


  if ((prevButtonState == HIGH && currentButtonState == LOW) || First_start ) {
    if(!Timer1_start){
      start=millis();
      First_start=HIGH;
      Timer1_start=HIGH;
    }
    Pin1.activate(start);
    Pin2.activate(start);
    Pin3.activate(start);
    check1();
  }
  

  if (prevButtonState == LOW && currentButtonState == HIGH || Second_start) {
    if(!Timer2_start){
      start2=millis();
      Second_start=HIGH;
      Timer2_start=HIGH;
    }
    Pin4.activate(start2);
    Pin5.activate(start2);
    Pin6.activate(start2);
    check2();
  }

  prevButtonState = currentButtonState;
}
