#ifndef ONEBREATH_H
#define ONEBREATH_H

void oneBreath(){
  long interval = 100;

    for(long i = 0; i<interval; i++){
    presRawTotal += analogRead(presAna);
    flowRawTotal += analogRead(flowAna);
    }

  
  //PRESSURE LINEAR INTERPOLATION AVERAGE
  float presVol = (presRawTotal * 5 / 1023)/interval;
  pres = presArr[1][0] + (presVol - presArr[0][0]) * ((presArr[1][1] - presArr[1][0]) / (presArr[0][1] - presArr[0][0]));
  if (pres < 0)
    pres = 0;

  //FLOW LINEAR INTERPOLATION AVERAGE
  float flowVol = (flowRawTotal * 5 / 1023)/interval;

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
  Serial.print(pres*30);
  Serial.print(" ");
  Serial.print(flow);
  Serial.println(" ");
 flowVol=0;
  presVol = 0;
  presRawTotal = 0;
  flowRawTotal = 0;
}

#endif