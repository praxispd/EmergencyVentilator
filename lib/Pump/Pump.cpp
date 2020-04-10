#include "Pump.h"
#include <Arduino.h>

Pump::Pump(int pin){
    Pump::ctrlPin = pin;
}

void Pump::setDutyCyc(int pin, int DC, int maxval) {
    Pump::dutyCycle = DC;
    digitalWrite(pin, DC);
}

int Pump::getDutyCyc() {
    return dutyCycle;
}
