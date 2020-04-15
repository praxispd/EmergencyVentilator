#include <Arduino.h>
#include <HardwareSerial.h>
#include <stdio.h>
#include <stdint.h>
#include "Control.h"
#include "Pump.h"
#include "EmergencyVentilator.h"

Control bpmControl = Control(anaBPM, limBPMLow, limBPMHigh);
// Control conPause = Control(anaPause);
Control powerControl = Control(anaPower, limPowerLow, limPowerHigh);

// Pump pumpPositive = Pump(ctrlPosPump);
// Pump pumpNegative = Pump(ctrlNegPump);

ISR(TIMER1_COMPA_vect) {
    switch (enmState) {
        case inhale:
            // Set exhale on pump action
            digitalWrite(ctrlHBrInA, LOW);
            digitalWrite(ctrlHBrInB, LOW);
            digitalWrite(ctrlHBrSel, LOW);
            digitalWrite(ctrlHBrEn, LOW);
            analogWrite(pwmInPump, 0);
            digitalWrite(ctrlOutPump, HIGH);

            // Open solenoid
            digitalWrite(ctrlSol, HIGH);

            // Set timer
            OCR1A = round((exhaleOnTime/1000.0)*timerCycles);
            // Set state
            enmState = exhaleOn; // skip pause for now
            break;

        case pause:
            // Set pause pump action
            digitalWrite(ctrlHBrInA, LOW);
            digitalWrite(ctrlHBrInB, LOW);
            digitalWrite(ctrlHBrSel, LOW);
            digitalWrite(ctrlHBrEn, LOW);
            analogWrite(pwmInPump, 0);
            digitalWrite(ctrlOutPump, HIGH);

            // Open solenoid
            digitalWrite(ctrlSol, HIGH);

            // Reset timer
            OCR1A = round((exhaleOnTime/1000.0)*timerCycles);
            // Set state
            enmState = exhaleOn;
            break;

        case exhaleOn:
            if (exhaleOffTime > 0) {
                // Set exhale off pump action
                digitalWrite(ctrlHBrInA, LOW);
                digitalWrite(ctrlHBrInB, LOW);
                digitalWrite(ctrlHBrSel, LOW);
                digitalWrite(ctrlHBrEn, LOW);
                analogWrite(pwmInPump, 0); // 0-100 -> 0-255
                digitalWrite(ctrlOutPump, LOW);

                // Reset timer
                OCR1A = round((exhaleOffTime/1000.0)*timerCycles);
                // Set state
                enmState = exhaleOff;
                break;
            }

        case exhaleOff:
            // Set inhale pump action
            digitalWrite(ctrlHBrInA, HIGH);
            digitalWrite(ctrlHBrInB, LOW);
            digitalWrite(ctrlHBrSel, HIGH);
            digitalWrite(ctrlHBrEn, HIGH);
            analogWrite(pwmInPump, (int) round(pumpLevel*2.55)); // 0-100 -> 0-255
            digitalWrite(ctrlOutPump, LOW);

            // Close solenoid
            digitalWrite(ctrlSol, LOW);

            // Reset timer
            OCR1A = round((inhaleTime/1000.0)*timerCycles);
            // Set state
            enmState = inhale;
            break;
    };
}

void setup() {

    // Initialize pins
    pinMode(ctrlHBrInA, OUTPUT);
    pinMode(ctrlHBrInB, OUTPUT);
    pinMode(ctrlHBrSel, OUTPUT);
    pinMode(ctrlHBrEn, OUTPUT);
    pinMode(ctrlSol, OUTPUT);
    pinMode(pwmInPump, OUTPUT);
    pinMode(ctrlOutPump, OUTPUT);
    // pinMode(anaBreath, INPUT);
    pinMode(anaBPM, INPUT);
    pinMode(anaPause, INPUT);
    pinMode(anaPower, INPUT);

    cli(); // disable all interrupts

    // Set clock speed
#ifdef _1MHZ
    CLKPR = (1 << CLKPCE);
    CLKPR = (1 << CLKPS2); // 1 MHz
#endif

    // Set PWM prescaler
    TCCR3B = TCCR3B & 0b11111000 | 0b00000001; // no prescaler
    // TCCR3B = TCCR3B & 0b11111000 | 0b00000010; // 8
    // TCCR3B = TCCR3B & 0b11111000 | 0b00000011; // 64
    // TCCR3B = TCCR3B & 0b11111000 | 0b00000100; // 256
    // TCCR3B = TCCR3B & 0b11111000 | 0b00000101; // 1024

    // TIMER1 interrupt
    TCCR1A = 0; // control register A
    TCCR1B = 0; // control register B
    TCNT1 = 0; // counter register
    OCR1A = timerCycles; // compare match register
    TCCR1B |= (1 << WGM12); // CTC mode (clear timer on compare)
#ifdef _1MHZ
    TCCR1B |= (1 << CS12); // 256 prescaler
#else
    TCCR1B |= (1 << CS12)|(1 << CS10); // 1024 prescaler
#endif
    TIMSK1 |= (1 << OCIE1A); //enable timer compare interrupt

    sei(); //enable all interrupts

    // Initialize postive pump
    digitalWrite(ctrlHBrInA, LOW);
    digitalWrite(ctrlHBrInB, LOW);
    digitalWrite(ctrlHBrSel, LOW);
    digitalWrite(ctrlHBrEn, LOW);
    analogWrite(pwmInPump, 0);

    // Initialize negative pump
    digitalWrite(ctrlOutPump, LOW);

    // Initialize solenoid closed
    digitalWrite(ctrlSol, LOW);

#ifdef DEBUG
#ifdef _1MHZ
    Serial.begin(153600);
#else
    Serial.begin(9600);
#endif
    Serial.println(F("Debug on"));
#endif
}

void loop() {

    // Absolute breath time
    // breathTime = (int) round(((float) map(analogRead(anaBreath), 0, 1023, limBreathLow, limBreathHigh)/1000)*15625);
    // pauseTime = (int) round(((float) map(analogRead(anaPause), 0, 1023, limPauseLow, limPauseHigh)/1000)*15625);

    // Breaths per minute
    static float BPM;

    /*
    It: Inhale Time (ms)
    Ip: Inhale Power (%)
    Iv: Inhale Volume

        Iv = It*Ip

    Et: Exhale Time (ms)
    Etp: Exhale Pump Time (ms)
    Etw: Exhale Power (%)
    Ev: Exhale Volume

        Et = Etp + Etw
        Ev = Etp*Ep

    To maintain equal water levels in each chamber:

        Iv = Ev
    */
    
    BPM = bpmControl.read();
    exhaleTime = (60/BPM - inhaleSecs)*1000;

    /*
    Limit input power when It > Et:

        Iv = Ev
        It*Ip = Etp*Ep
        1000*Ip = Etp*100
        Etp = 10*Ip

        Et >= Etp
        Et >= 10*Ip
        Ip <= Et/10
    */
    pumpLevel = min(exhaleTime/10.0, powerControl.read());

    exhaleOnTime = min(round(pumpLevel*10), exhaleTime);
    exhaleOffTime = exhaleTime - exhaleOnTime;

    LOG("\nBPM: ");
    LOGLN(BPM, 2);
    LOG("Power: ");
    LOGLN(pumpLevel, 2);
    LOG("Inhale Time: ");
    LOGLN(inhaleTime);
    LOG("Exhale Time: ");
    LOGLN(exhaleTime);
    LOG("Exhale On Time: ");
    LOGLN(exhaleOnTime);
    LOG("Exhale Off Time: ");
    LOGLN(exhaleOffTime);
    LOG("Inhale Volume: ");
    LOGLN(pumpLevel*1000, 0);
    LOG("Exhale Volume: ");
    LOGLN(100*exhaleOnTime);

#ifdef _1MHZ
    delay(50);
#else
    delay(1000);
#endif
}