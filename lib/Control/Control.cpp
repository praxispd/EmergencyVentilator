#include <Arduino.h>
#include "Control.h"

    Control::Control(int pin, float minOut = 0, float maxOut = 100, int minRead = 0, int maxRead = 1023) {
        Control::pin = pin;
        Control::minOut = minOut;
        Control::maxOut = maxOut;
        Control::minRead = minRead;
        Control::maxRead = maxRead;
    }

    void Control::setMinOut(int value) {
        minOut = value;
    }
    void Control::setMaxOut(int value) {
        maxRead = value;
    }
    void Control::setMaxRead(int value) {
        maxRead = value;
    }
    void Control::setMinRead(int value) {
        minRead = value;
    }

    float Control::read() {
        //convert range of read into range of output
        lastRead = (analogRead(pin) - minRead)*(maxOut - minOut)/(maxRead - minRead) + minOut;
        return lastRead;
    }

    float Control::avgRead(int reads, int delayTime) {
        int sum = 0;
        for (int i = 0; i < reads; i++) {
            sum += read();
            delay(delayTime);
        }
        lastRead = sum/reads;
        return lastRead;
    }

    float Control::safeRead() {
        //reads and checks values havent changed too much
        float output;
        int rawIn = analogRead(pin); 
        output = ((float)rawIn*(float)(minRead - maxRead)/((float)minOut - maxOut)) + (float)minOut;

        if (lastRead == NULL) {
            lastRead = output;
            delay(100);
            float checkVal = avgRead(10, 10);
            if (abs(checkVal - lastRead) > lastRead*0.1) {
                output = read();
            }
        } else if (abs(output - lastRead) > (0.1*lastRead)) {
            output = read();
        }
        lastRead = output;
        return output;
    }
