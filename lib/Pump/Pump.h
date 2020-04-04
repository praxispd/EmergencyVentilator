#ifndef PUMP_H
#define PUMP_H
#include <Arduino.h>

class Pump{
  public:
    Pump(int pin);

    void setDutyCyc(int pin, int DC);
    int getDutyCyc();

  private:
    int dutyCycle;
    int DCConvert(int DC, int max = 1023);
};


#endif