#include <Arduino.h>
#include <HardwareSerial.h>
#include <globalVars.h>
#include <pins.h>
#include <stdio.h>
#include <stdint.h>
#include <Control.h>
#include <Pump.h>

void setup(){
Serial.begin(115200);
Serial.println("setuptest");

Pump pump_positivePressure = Pump(1);
Pump pump_NegitivePressure = Pump(2);

Control Control_BreathPerMin = Control(anaBPMin);
Control Control_BreathVolume = Control(anaBVol);
Control Control_BreathPause = Control(anaBPause);


}

void loop(){
    delay(1000);
    Serial.println("test");

}