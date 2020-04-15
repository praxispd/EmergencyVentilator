#ifndef EMERGENCYVENTILATOR_H
#define EMERGENCYVENTILATOR_H

// Global settings
#define DEBUG

// Clock rate
#define _1MHZ

#ifdef DEBUG
    #define LOGLN(...) Serial.println(__VA_ARGS__)
    #define LOG(...) Serial.print(__VA_ARGS__)
#else
    #define LOGLN(...)
    #define LOG(...)
#endif

// Pin assignment
const int ctrlHBrInA = 40;
const int ctrlHBrInB = 41;
const int ctrlHBrSel = 42;
const int ctrlHBrEn = 8;
const int ctrlSol = 52;
const int ctrlOutPump = 53;
const int pwmInPump = 2;
// const int ctrlHumidifier;
// const int ctrlHeater;
// const int anaHumid;
// const int anaHeater;
// const int anaBreath = A15;
const int anaBPM = A15;
const int anaPause = A14;
const int anaPower = A13;

// Machine state
enum State {
    inhale,
    exhaleOn, // Pump on
    exhaleOff, // Pump off
    pause
};

State enmState = inhale; // machine state

// Inhale length (s)
const unsigned int inhaleSecs = 1;

// Control limits for breaths per minute
#ifdef _1MHZ
const unsigned int limBPMLow = 6; 
#else
const unsigned int limBPMLow = 12; 
#endif
const unsigned int limBPMHigh = 40;

// Control limits for pump power (%)
const unsigned int limPowerLow = 10;
const unsigned int limPowerHigh = 100;

// Control limits for breath/pause lengths (ms)
unsigned int limBreathLow = 500;
unsigned int limBreathHigh = 3000;
unsigned int limPauseLow = 500;
unsigned int limPauseHigh = 3000;

// Timer clock cycles in 1 second
#ifdef _1MHZ
const unsigned int timerCycles = 3906; // timer prescaler 256 (1M/256)
#else
const unsigned int timerCycles = 15625; // timer prescaler 1024 (16M/1024)
#endif

// Timings (ms)
// unsigned int breathTime = 0;
// unsigned int pauseTime = 0; 
unsigned int inhaleTime = inhaleSecs*1000;
unsigned int exhaleTime = inhaleSecs*1000;
unsigned int exhaleOnTime = inhaleSecs*1000;
unsigned int exhaleOffTime = 0;

// Pump power level
float pumpLevel = 100;

#endif