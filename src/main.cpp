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

 //------------------initialize ISPs------------------//
  //initialize TIMER1 for blink
  noInterrupts(); //disable all interrupts
  TCCR1A = 0; //control register A
  TCCR1B = 0; //control register B
  TCNT1 = 0; //counter register
  OCR1A = 62500; //compare match register (16MHz/64 -> 8Hz)
  TCCR1B |= (1 << WGM12); //CTC mode (clear timer on compare)
  TCCR1B |= (1 << CS12); //256 prescaler
  TIMSK1 |= (1 << OCIE1A); //enable timer compare interrupt
  interrupts(); //enable all interrupts



}

void loop(){
    //read freq vol and control levels
    BPMin = Control_BreathPerMin.safeRead();
    BVol = Control_BreathVolume.safeRead();
    BPause = Control_BreathPause.safeRead();
    
    //calc pump DCs

    //Set pump Interrupt

}