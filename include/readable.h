#ifndef READABLE_H
#define READABLE_H

void func(){
  while(flowRaw<10 && presRaw<10){
    flowRaw = analogRead(flowAna);
    presRaw = analogRead(presAna);
  }
  while (flowRaw>=10 || presRaw>=10) {
    presRaw = analogRead(presAna);
    flowRaw = analogRead(flowAna);
    presRawTotal += presRaw*presRaw;
    flowRawTotal += flowRaw*presRaw;
    if(presRaw>presRawMax) presRawMax = presRaw;
    if(presRaw<presRawMin) presRawMin = presRaw;
    if(flowRaw>flowRawMax) flowRawMax = flowRaw;
    if(flowRaw<flowRawMin) flowRawMin = flowRaw;
    countr ++;    
  }
  
  //PRESSURE LINEAR INTERPOLATION AVERAGE
  float presVol = sqrt((presRawTotal * 5 / 1023) / countr);
  //Serial.print("PresVol: ");
  //Serial.print(presVol);
  //Serial.println();
  pres = presArr[1][0] + (presVol - presArr[0][0]) * ((presArr[1][1] - presArr[1][0]) / (presArr[0][1] - presArr[0][0]));
  if (pres < 0){
    pres = 0;
    } 
    #ifdef CSV
    Serial.print(pres,3);
    Serial.print(",");
    #endif
    #ifdef HUMAN
    Serial.print("Pres over 1 sec (PSI): AVG - ");
    Serial.print(pres, 3);
    #endif
  presRawTotal=0;
  pres=0;

  //PRESSURE LINEAR INTERPOLATION MIN
  presVol = (presRawMin * 5 / 1023);
  pres = presArr[1][0] + (presVol - presArr[0][0]) * ((presArr[1][1] - presArr[1][0]) / (presArr[0][1] - presArr[0][0]));
  if (pres < 0){
    pres = 0;
    } 
    #ifdef CSV
    Serial.print(pres,3);
    Serial.print(",");
    #endif
    #ifdef HUMAN
  Serial.print("  MIN - ");
  Serial.print(pres, 3);
    #endif


  //PRESSURE LINEAR INTERPOLATION MAX
  presVol = (presRawMax * 5 / 1023);
  pres = presArr[1][0] + (presVol - presArr[0][0]) * ((presArr[1][1] - presArr[1][0]) / (presArr[0][1] - presArr[0][0]));
  if (pres < 0){
    pres = 0;
    } 
    #ifdef CSV
    Serial.print(pres,3);
    Serial.print(",");
    #endif
  #ifdef HUMAN
  Serial.print("  MAX - ");
  Serial.print(pres, 3);
  Serial.println();

#endif
    
  //FLOW LINEAR INTERPOLATION AVERAGE
  float flowVol = sqrt((flowRawTotal * 5 / 1023) / countr);
  //Serial.print("FlowVol: ");
  //Serial.print(flowVol);
  //Serial.println();
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
  #ifdef CSV
    Serial.print(flow,3);
    Serial.print(",");
  #endif
  #ifdef HUMAN
  Serial.print("Flow over 1 sec (SLPM): AVG - ");
  Serial.print(flow,3);
  #endif

  //FLOW LINEAR INTERPOLATION MIN
  flowVol = (flowRawMin * 5 / 1023);
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
  #ifdef CSV
    Serial.print(flow,3);
    Serial.print(",");
  #endif
  #ifdef HUMAN
  Serial.print("  MIN - ");
  Serial.print(flow,3);
  #endif

  //FLOW LINEAR INTERPOLATION MAX
  flowVol = (flowRawMax * 5 / 1023);
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
  iterations++;
  #ifdef CSV
    Serial.print(flow,3);
    Serial.print(",");
    Serial.print((iterations*time/1000));
    Serial.println(";");
  #endif
  #ifdef HUMAN
  Serial.print("  MAX - ");
  Serial.print(flow,3);
  Serial.println();
  Serial.println();
  Serial.println("------------------------------------------------------------------------------");
  Serial.println();
  Serial.print(" The elapsed time is ");
  Serial.print(iterations*time/1000,3);
  Serial.println(" seconds");
  #endif
  flowRaw = 0;
  flowRawMax = 0;
  flowRawMin = 0;
  presRaw = 0;
  presRawMin = 0;
  presRawMax = 0;
  countr = 0;
  timer = millis();
  presRawTotal=0;
  flowRawTotal=0;
}

#endif