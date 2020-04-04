
#ifndef PUMP_TEST_H
#define PUMP_TEST_H


//test should always be the function name
bool pump_test(){

    T_LOG(F("Begin testing on: "));
    T_LOGLN(__FUNCTION__);
    //test 1
    if(true)
        T_LOGLN("#1- pass <relevant info> test");
    
    //test 2
    if(false)
        T_LOGLN("#2- pass <relevant info> test");
    

}
#endif