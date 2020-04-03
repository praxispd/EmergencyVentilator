#include <Arduino.h>
#include <HardwareSerial.h>
#include <globalVars.h>
#include <pins.h>
#include <stdio.h>
#include <stdint.h>
#include <Control.h>

void setup(){
Serial.begin(115200);
Serial.println("setuptest");
}

void loop(){
    delay(1000);
    Serial.println("test");

}