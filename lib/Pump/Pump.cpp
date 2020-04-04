#include <Pump.h>
#include <Arduino.h>

Pump::Pump(int pin){
    Pump::ctrlPin = pin;
}

void Pump::setDutyCyc(int pin, int DC){
    Pump::dutyCycle = DC;
    digitalWrite(pin,DC);    
}

int Pump::getDutyCyc(){
    return dutyCycle;
}

