#ifndef PUMP_H
#define PUMP_H
#include <Control.h>

class Pump{
  public:

    Pump(int pin);
  
    void setDutyCyc(int pin, int DC, int maxval = 1023);
    int getDutyCyc();  
    void monitor();

  private:
    int ctrlPin;
    int dutyCycle;
    int DCConvert(int DC, int max = 1023);
};


#endif