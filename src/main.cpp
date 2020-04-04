#include <Arduino.h>
#include <HardwareSerial.h>
#include <globalVars.h>
#include <pins.h>
#include <stdio.h>
#include <stdint.h>
#include <Control.h>
#include <Pump.h>
#ifdef TEST
    #include "test/test.h"
#endif

#ifdef DEBUG
    #define LOGLN(...) Serial.println(__VA_ARGS__)
    #define LOG(...) Serial.print(__VA_ARGS__)
#else
    #define LOGLN(...)
    #define LOG(...)
#endif

void setup(){
#ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Debug on");
#endif
Pump pump_positivePressure = Pump(1);
Pump pump_NegitivePressure = Pump(2);

Control Control_BreathPerMin = Control(anaBPMin);
Control Control_BreathVolume = Control(anaBVol);
Control Control_BreathPause = Control(anaBPause);

#ifdef TEST
    runTestCode();
#endif

}

void loop(){
    delay(1000);
    Serial.println("test");

}