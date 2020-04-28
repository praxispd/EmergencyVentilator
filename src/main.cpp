#include <arduino.h>
#include <HardwareSerial.h>
#include <globals.h>
#include <readable.h>
#include <oneBreath.h>


void setup() {
  Serial.begin(9600);
  timer = millis();
  #ifdef CSV
  Serial.print("reading interval is: ");
  Serial.print(time);
  Serial.println("miliseconds");
  Serial.println("Pres-Avg,Pres-Min,Pres-Max,Flow-Avg,Flow-Min,Flow-Max,elapsed-time;");
  #endif
  #ifdef HUMAN
  Serial.print("reading interval is: ");
  Serial.print(time);
  Serial.println("miliseconds");
  Serial.println("this is the human readable format");
  #endif
  #ifdef ERROR
  Serial.print("reading interval is: ");
  Serial.print(time);
  Serial.println("miliseconds");
  Serial.println("this is the error reporting only format");
  #endif
  #ifdef ONEBREATH
  Serial.println("pressure flow");
  Serial.println(" ");
  #endif
}

void loop() {
  #ifndef ERROR
 // func();
  #endif
  #ifdef ERROR
  Error_func();
  #endif
  #ifdef ONEBREATH
  oneBreath();
  #endif
}
