#ifndef E_VENT_H
#define E_VENT_H

//settings
#define DEBUG
#define TEST


//place device specific global vars here

float BPMin;//breaths per minute
float BVol; //volume of a breath (pressure)
float BPause; //pause between breaths

enum state{
    inhale,
    exhale,
    pause
};

#endif