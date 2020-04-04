#include <Pump.h>
#include <Arduino.h>

Pump::Pump(int pin){

}

void Pump::setDutyCyc(int pin, int DC){
    Pump::dutyCycle = DC;
    digitalWrite(pin, DCConvert(DC));    
}

int Pump::getDutyCyc(){
    return dutyCycle;
}

int Pump::DCConvert(int DC, int maxVal=1023){
    return DC * maxVal/100;
}