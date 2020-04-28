#ifndef GLOBALS_H
#define GLOBALS_H

//#define CSV
#define HUMAN
//#define ERROR
//#define ONEBREATH

const int presAna = A0;//pressure read pin
const int flowAna = A1;//flow read pin

long time = 300;
long iterations = 0;
float flowRaw = 0;
float flowRawTotal = 0;
float flowRawMin = 0;
float flowRawMax = 0;

float presRaw = 0;
float presRawTotal = 0;
float presRawMin = 0;
float presRawMax = 0;

float pres = 0;
float flow = 0;
float countr = 0;
unsigned long timer;

float flowArr[2][6] = { {1, 2.45, 3.51, 4.2, 4.66, 5 }, 
                        {0, 10,   20,   30,  40,   50} };

float presArr[2][2] = { {0.5, 4.5},
                        {0,   2.17} };
#endif