#ifndef GLOBALS_H
#define GLOBALS_H

#define CSV
//#define HUMAN
//#define ERROR
//#define GRAPH

int threshold = 1; //SLPN

const int presAna = A0;//pressure read pin
const int flowAna = A1;//flow read pin


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



float calcPres(float p_raw){
  float presVol = p_raw *5/1023;
  return presArr[1][0] + (presVol - presArr[0][0]) * ((presArr[1][1] - presArr[1][0]) / (presArr[0][1] - presArr[0][0]));
}

float calcFlow(float f_raw){
  float flowVol = f_raw*5/1023;
  for (int i = 0; i < sizeof(flowArr) / (sizeof(flowArr[0][0])); i++) {
      if (flowVol >= flowArr[0][i] && flowVol <= flowArr[0][i+1]) {
        float x1 = flowArr[0][i];
        float x2 = flowArr[0][i+1];
        float y1 = flowArr[1][i];  
        float y2 = flowArr[1][i+1]; 
        flow = y1 + (flowVol - x1)*((y2 - y1)/(x2 - x1)); 
        break;
      }
    if (flowVol < flowArr[0][0]){
      flow = 0;
      break;    
      }
    if(flowVol > flowArr[0][sizeof(flowArr[0][0])]){
      flow = 50;
      break;
      }   
    }  
  return flow;
}

void smoothread(float &pres, float &flow, unsigned long smoothing_time){
  //returns smoothed readings
  unsigned long start = millis();
  int countr = 0;
  float pres_val = 0;
  float flow_val = 0;
  while((millis()-start)<smoothing_time){
    pres_val+= pow(analogRead(presAna),2);
    flow_val+= pow(analogRead(flowAna),2);
    countr ++;
  }
  float tempPres = calcPres(sqrt(pres_val/countr)); 
  pres = tempPres;
  float tempFlow = calcFlow(sqrt(flow_val/countr));
  flow = tempFlow;
  
}

#endif