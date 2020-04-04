#ifndef test_h
#define test_h
//this is for adding in test code
#define T_LOG(...) Serial.print(__VA_ARGS__)
#define T_LOGLN(...) Serial.println(__VA_ARGS__)

void serialFlush(){
  while(Serial.available())
    Serial.read();
}

//for tests that require manual checking
bool manualResp(char* output, char input = ""){
  T_LOGLN(output);
  
  serialFlush();
  while (Serial.available()==0);
  
  delay(100);
  return (input == Serial.read());
}


//----------------------uncomment required test code headers----------------------
#include "pumps.h"
#include "controls.h"
//--------------------------------------------------------------------------------

void runTestCode(){
    T_LOGLN(F("testcode running"));
    T_LOGLN(F("====================================================================================="));

    if (pump_test())
      T_LOGLN("test pass");
    else
      T_LOGLN("test Fail");

    T_LOGLN(F("====================================================================================="));

    if (controls_test())
      T_LOGLN("test pass");
    else
      T_LOGLN("test Fail");

    T_LOGLN(F("====================================================================================="));
    T_LOGLN(F("testcode DONE"));
    while(1);
}
#endif