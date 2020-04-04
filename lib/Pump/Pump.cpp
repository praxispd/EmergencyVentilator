#include <Pump.h>
#include <Arduino.h>

Pump::Pump(int pin){
    Pump::ctrlPin = pin;
}

void Pump::setDutyCyc(int pin, int DC, int maxval = 1023){
    Pump::dutyCycle = DC;
    digitalWrite(pin, DCConvert(DC, maxval));    
}

int Pump::getDutyCyc(){
    return dutyCycle;
}

int Pump::DCConvert(int DC, int maxVal=1023){
    return DC * maxVal/100;
}
