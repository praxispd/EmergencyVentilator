#ifndef READABLE_H
#define READABLE_H
#include <globals.h>



void func(){
  int counter = 0;
  float pres_avg = 0;
  float pres_min = 0;
  float pres_max = 0;
  float flow_avg = 0;
  float flow_min = 0;
  float flow_max = 0;
  float volume = 0;
  float flow = 0;
  float pres =0;
  unsigned long inhale_duration = 0;
  unsigned long exhale_duration = 0;
  unsigned long integralTimer = 0;
  unsigned long timer = millis();
  //start exhale timer
  shortDur: ;
  while(flow<threshold){
    smoothread(pres,flow,10);
  }
  exhale_duration = millis()-timer;
  if (exhale_duration < 200)
    return;
  timer = millis();
  while(flow>=threshold){
    integralTimer = millis();
    smoothread(pres,flow,10);
    volume+= (flow/60)*(float)(millis()-integralTimer);
    if (pres<pres_min) pres_min = pres;
    if (pres>pres_max) pres_max = pres;
    if (flow<flow_min) flow_min = flow;
    if (flow>flow_max) flow_max = flow;
    pres_avg += pow(pres,2);
    flow_avg += pow(flow,2);
    counter +=1;
  }
  inhale_duration = millis()-timer;
  if(inhale_duration<100)
    goto shortDur;
  flow_avg = sqrt(flow_avg/counter);
  pres_avg = sqrt(pres_avg/counter);

  #ifdef HUMAN
    Serial.println("------------------------------------------------------------");
    Serial.print("inhale time(ms): ");
    Serial.print(inhale_duration);
    Serial.print("    exhale time(ms): ");
    Serial.println(exhale_duration);
    Serial.print("breath volume is: ");
    Serial.print(volume);
    Serial.println("(ml)");
    Serial.print("Flow: min:");
    Serial.print(flow_min);
    Serial.print("   avg: ");
    Serial.print(flow_avg);
    Serial.print("    max:");
    Serial.println(flow_max);
    Serial.print("Pres: min:");
    Serial.print(pres_min);
    Serial.print("   avg: ");
    Serial.print(pres_avg);
    Serial.print("    max:");
    Serial.println(pres_max);
  #endif 
  #ifdef CSV
    Serial.print(inhale_duration);
    Serial.print(",");
    Serial.print(exhale_duration);
    Serial.print(",");
    Serial.print(volume);
    Serial.print(",");
    Serial.print(flow_min);
    Serial.print(",");
    Serial.print(flow_avg);
    Serial.print(",");
    Serial.print(flow_max);
    Serial.print(",");
    Serial.print(pres_min);
    Serial.print(",");
    Serial.print(pres_avg);
    Serial.print(",");
    Serial.println(pres_max);
  #endif
  
/*

  flowRaw = 0;
  presRaw = 0;
  countr = 0;
  presRawMax = 0;
  flowRawMax = 0;
  unsigned long smoothingTime=10;
  //while pressure and flow are below threshold you are in exhale
  while(flowRaw<threshold && presRaw<threshold){
    timer = millis();
    while((millis()-timer)<smoothingTime){
      presRaw += pow(analogRead(presAna),2);
      flowRaw += pow(analogRead(flowAna),2);
      countr ++;
    }
    presRaw = sqrt(presRaw/countr);
    flowRaw = sqrt(flowRaw/countr);    
    
  }
  unsigned long exhale_duration = millis()-timer;


  //while pressure or flow is above threshold you are in inhale
  timer = millis();
  while (flowRaw>=threshold || presRaw>=threshold) {
    timer = millis();
    while((millis()-timer)<smoothingTime){
      presRaw += pow(analogRead(presAna),2);
      flowRaw += pow(analogRead(flowAna),2);
      countr ++;
    }
    presRaw = sqrt(presRaw/countr);
    flowRaw = sqrt(flowRaw/countr);
  }
  if(presRaw>presRawMax) presRawMax = presRaw;
  if(presRaw<presRawMin) presRawMin = presRaw;
  if(flowRaw>flowRawMax) flowRawMax = flowRaw;
  if(flowRaw<flowRawMin) flowRawMin = flowRaw;
  
  unsigned long inhale_duration = millis()-timer;

  if (inhale_duration<100 || exhale_duration <100)
    return;

  #ifdef HUMAN
    Serial.print("inhale duration was ");
    Serial.print(inhale_duration);
    Serial.println("ms");
    Serial.print("exhale duration was ");
    Serial.print(exhale_duration);
    Serial.println("ms");
  #endif
  #ifdef CSV
    Serial.print(inhale_duration);
    Serial.print(",");
    Serial.print(exhale_duration);
    Serial.print(",");
  #endif
  //PRESSURE LINEAR INTERPOLATION AVERAGE

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
  
  #ifdef CSV
    Serial.print(flow,3);
    Serial.print(",");
    float flowHolder = flow;
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
    Serial.print(flowHolder*inhale_duration/60);
    Serial.println(";");
  #endif
  #ifdef HUMAN
  Serial.print("  MAX - ");
  Serial.print(flow,3);
  Serial.println();
  Serial.print("Breath volume:");
  Serial.print(flow*inhale_duration/60);
  Serial.println("ml");
  Serial.println();
  Serial.println("------------------------------------------------------------------------------");
  #enif
*/
}

#endif