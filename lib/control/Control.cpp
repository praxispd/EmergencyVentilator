#include <Control.h>
#include <Arduino.h>

    Control::Control(int pin, int minRead=0,int maxRead=1023, float minOut=0, float maxOut=100){
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
    float Control::read(){
        float output;
        int rawIn = digitalRead(ctrlPin); 
        //convert range of read into range of output
        output = ((float)rawIn*(float)(minRead-maxRead)/((float)minOut-maxOut))+(float)minOut;
        lastval = output;
        return output;
    }

    float Control::avgRead(int reads, int delayTime){
        int sum = 0;
        for(int i = 0; i < reads; i++){
            sum+=read();
            delay(delayTime);
        }
        lastval = sum/reads;
        return lastval;
    }

    float Control::safeRead(){
        //reads and checks values havent changed too much
        float output;
        int rawIn = digitalRead(ctrlPin); 
        output = ((float)rawIn*(float)(minRead-maxRead)/((float)minOut-maxOut))+(float)minOut;
        if(lastval == NULL){
            lastval = output;
            delay(500);
            float checkVal = avgRead(10, 50);
            if(abs(checkVal - lastval)>lastval*0.1){
                read();
            }
        }
        if(abs(output-lastval)>(0.1*lastval)){
            read();
        }
        lastval = output;
        return output;
    }
