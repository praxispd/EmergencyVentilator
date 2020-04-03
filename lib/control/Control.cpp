#include <Control.h>
#include <Arduino.h>

    Control::Control(int pin, int minRead,int maxRead, float minOut, float maxOut){
        Control::minRead = minRead;
        Control::maxRead = maxRead;
        Control::minOut = minOut;
        Control::maxOut = maxOut;
    }

    void Control::setMaxRead(int value){
        maxRead = value;
    }
    void Control::setMinRead(int value){
        minRead = value;
    }
    void Control::setMinOut(int value){
        minOut = value;
    }
    void Control::setMaxOut(int value){
        maxRead = value;
    }
    float Control::read(int pin){
        float output;
        int rawIn = digitalRead(pin); 
        output = ((float)rawIn*(float)(minRead-maxRead)/((float)minOut-maxOut))+(float)minOut;
        return output;
    }

    float Control::avgRead(int pin, int reads){
        int sum = 0;
        for(int i = 0; i < reads; i++){
            sum+=read(pin);
        }
        return sum/reads;
    }
