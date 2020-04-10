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
    switch (mchState) {
        case inhale:
            // Set exhale pump action
            analogWrite(pwmPosPump, 0);
            analogWrite(pwmNegPump, 100);

            // Open solenoid
            digitalWrite(ctrlSol, HIGH);

            // Set timer
            OCR1A = exhaleTime;
            // Set state
            mchState = exhale; // skip pause for now
        break;

        case pause:
            // Set pause pump action
            analogWrite(pwmPosPump, 0);
            analogWrite(pwmNegPump, 0);

            // Open solenoid
            digitalWrite(ctrlSol, HIGH);

            // Reset timer
            OCR1A = exhaleTime;
            // Set state
            mchState = exhale;
        break;

        case exhale:
            // Set exhale pump action
            analogWrite(pwmPosPump, (int) round(pumpLevel*2.55)); // 0-100 -> 0-255
            analogWrite(pwmNegPump, 0);

            // Close solenoid
            digitalWrite(ctrlSol, LOW);

            // Reset timer
            OCR1A = inhaleTime;
            // Set state
            mchState = inhale;
        break;
    };
}

void setup() {

    // Initialize pins
    pinMode(ctrlHBrInA, OUTPUT);
    pinMode(ctrlHBrInB, OUTPUT);
    pinMode(ctrlHBrSel, OUTPUT);
    pinMode(ctrlSol, OUTPUT);
    pinMode(pwmPosPump, OUTPUT);
    pinMode(pwmNegPump, OUTPUT);
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

    // Set PWM prescaler to 0
    // TCCR3A = (1 << CS30);

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
    digitalWrite(ctrlHBrInA, HIGH);
    digitalWrite(ctrlHBrInB, LOW);
    digitalWrite(ctrlHBrSel, HIGH);
    analogWrite(pwmPosPump, 0);

    // Initialize negative pump
    analogWrite(pwmNegPump, 0);

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

    static float BPM;
    
    BPM = bpmControl.read();
    exhaleTime = (60/BPM - inhaleSecs)*timerCycles;

    pumpLevel = round(powerControl.read());

    LOG("\nBPM: ");
    LOGLN(BPM, 2);
    LOG("Power: ");
    LOGLN(pumpLevel);

#ifdef _1MHZ
    delay(50);
#else
    delay(1000);
#endif
}