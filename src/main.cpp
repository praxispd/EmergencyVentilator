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

//create control objects
Control Control_BreathPerMin = Control(anaBPMin);
Control Control_BreathVolume = Control(anaBVol);
Control Control_BreathPause = Control(anaBPause);

//create pump objects
Pump pump_positivePressure = Pump(ctrlPosPump);
Pump pump_negitivePressure = Pump(ctrlNegPump);




void setup(){
#ifdef DEBUG
    Serial.begin(115200);
    Serial.println(F("Debug on"));
#endif

#ifdef TEST
    runTestCode();
#endif


 //------------------initialize ISRs------------------//

}

void loop(){
    //read freq vol and control levels
    BPMin = Control_BreathPerMin.safeRead();
    BVol = Control_BreathVolume.safeRead();
    BPause = Control_BreathPause.safeRead();
    
    //calc pump DCs

    //Set pump Interrupt

}