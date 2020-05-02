#include <arduino.h>
#include <HardwareSerial.h>
#include <globals.h>
#include <readable.h>
#include <graph.h>


void setup() {
  Serial.begin(9600);
  timer = millis();
  #ifdef CSV
  Serial.println("inhale_duration(ms),exhale_duration(ms),volume(ml),Flow-Min,Flow-Avg,Flow-Max,Pres-Min,Pres-Avg,Pres-Max");
  #endif
  #ifdef HUMAN
  Serial.println("this is the human readable format");
  #endif
  #ifdef ERROR
  Serial.print("reading interval is: ");
  Serial.print(time);
  Serial.println("miliseconds");
  Serial.println("this is the error reporting only format");
  #endif
  #ifdef GRAPH
  Serial.println("pressure flow");
  Serial.println(" ");
  #endif
}

void loop() {
  #ifdef HUMAN
    func();
  #endif
  #ifdef CSV
    func();
  #endif
  #ifdef ERROR
  Error_func();
  #endif
  #ifdef GRAPH
  graph();
  #endif
}
