#ifndef CONTROLS_TEST_H
#define CONTROLS_TEST_H

const int butn = 19;
const int ctrlLED = 37;
int state = LOW;

volatile bool led = false;
volatile int ctr = 0;

void TEST_ISR(){
    detachInterrupt(digitalPinToInterrupt(butn));
    led = true;
    ctr ++;
}

//test should always be the function name
bool controls_test(){
    pinMode(butn,INPUT);
    pinMode(ctrlLED, OUTPUT);

    T_LOG(F("Begin testing on: ")); T_LOGLN(__FUNCTION__);
    T_LOGLN(PC0);
    digitalWrite(37,HIGH);
    T_LOGLN(PC0);

    //test 1
    attachInterrupt(digitalPinToInterrupt(butn),TEST_ISR,RISING);
    while(1){
        if (led){
            T_LOG("switche ");
            led = false; 
            T_LOGLN(ctr);
            if(state == LOW) state = HIGH;
            else state = LOW;
            digitalWrite(ctrlLED ,state);
            delay(1000);
            attachInterrupt(digitalPinToInterrupt(butn),TEST_ISR,RISING);
        }
    }
    if(true)
        T_LOGLN("#1- pass <relevant info> test");
    
    //test 2
    if(false)
        T_LOGLN("#2- pass <relevant info> test");
    

}

#endif