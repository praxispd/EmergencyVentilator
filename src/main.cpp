#include <Arduino.h>
#include <HardwareSerial.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "Control.h"
#include "Pump.h"
#include "EmergencyVentilator.h"

Control bpmControl = Control(anaBPM, limBPMLow, limBPMHigh);
// Control conPause = Control(anaPause);
Control powerControl = Control(anaPower, limPowerLow, limPowerHigh);

// Pump pumpPositive = Pump(ctrlPosPump);
// Pump pumpNegative = Pump(ctrlNegPump);

ISR(TIMER1_COMPA_vect) {
    newState = true;
    // Advance state and reset timer counter
    switch (enmState) {
        case idle: break;
        case inhale:
            OCR1A = round((exhaleOnTime/1000.0)*timerCycles);
            LOG("ExhaleOn time: ");
            LOGLN(exhaleOnTime);
            enmState = exhaleOn; // skip pause for now
            break;
        case pause:
            OCR1A = round((exhaleOnTime/1000.0)*timerCycles);
            enmState = exhaleOn;
            break;
        case exhaleOn:
            // Skip to inhale if no exhale off time
            if (exhaleOffTime > 0) {
                OCR1A = round((exhaleOffTime/1000.0)*timerCycles);
                LOG("ExhaleOff time: ");
                LOGLN(exhaleOffTime);
                enmState = exhaleOff;
                break;
            }
        case exhaleOff:
            OCR1A = round((inhaleTime/1000.0)*timerCycles);
            LOG("Inhale time: ");
            LOGLN(inhaleTime);
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

    // Temporary toggle switch
    pinMode(3, INPUT);

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
#ifdef MASTER
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
#else
    TIMSK1 |= (0 << OCIE1A); // disable timer compare interrupt
#endif

    sei(); //enable all interrupts

    // Turn off pumps
    digitalWrite(ctrlHBrInA, LOW);
    digitalWrite(ctrlHBrInB, LOW);
    digitalWrite(ctrlHBrSel, LOW);
    digitalWrite(ctrlHBrEn, LOW);
    analogWrite(pwmInPump, 0);
    digitalWrite(ctrlOutPump, LOW);
    // Close solenoid 
    digitalWrite(ctrlSol, LOW);

#ifdef DEBUG
#ifdef _1MHZ
    Serial.begin(153600);
#else
    Serial.begin(9600);
#endif
    Serial.println(F("Debug on"));
#endif

    // Serial communication to other MCU
    Serial1.begin(9600);
    Serial1.setTimeout(300);

#ifndef MASTER
    delay(5000);
#endif
}

void loop() {

    // Send/receive machine state and TIMER1 counter register value
#ifdef MASTER
    // Send every 200 ms
    static unsigned int sendTimer = millis();
    if ((millis() - sendTimer) > 200) {
        sendTimer = millis();
        if (digitalRead(3) == HIGH) { // Temporary toggle switch for testing
            sendData();
        }
    }
#else
    if (!failover && !receiveData()) {
        failover = true;
        enmState = enmMasterState;
        newState = true;
        TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    }
#endif

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

    // LOG("\nBPM: ");
    // LOGLN(BPM, 2);
    // LOG("Power: ");
    // LOGLN(pumpLevel, 2);
    // LOG("Inhale Time: ");
    // LOGLN(inhaleTime);
    // LOG("Exhale Time: ");
    // LOGLN(exhaleTime);
    // LOG("Exhale On Time: ");
    // LOGLN(exhaleOnTime);
    // LOG("Exhale Off Time: ");
    // LOGLN(exhaleOffTime);
    // LOG("Inhale Volume: ");
    // LOGLN(pumpLevel*1000, 0);
    // LOG("Exhale Volume: ");
    // LOGLN(100*exhaleOnTime);

    // Peripheral control
    if (newState) {
        newState = false;
        switch (enmState) {
            case idle:
                // Turn off pumps
                digitalWrite(ctrlHBrInA, LOW);
                digitalWrite(ctrlHBrInB, LOW);
                digitalWrite(ctrlHBrSel, LOW);
                digitalWrite(ctrlHBrEn, LOW);
                analogWrite(pwmInPump, 0);
                digitalWrite(ctrlOutPump, LOW);
                // Close solenoid 
                digitalWrite(ctrlSol, LOW);
                break;

            case inhale:
                // Turn on inhale pump
                digitalWrite(ctrlHBrInA, HIGH);
                digitalWrite(ctrlHBrInB, LOW);
                digitalWrite(ctrlHBrSel, HIGH);
                digitalWrite(ctrlHBrEn, HIGH);
                analogWrite(pwmInPump, (int) round(pumpLevel*2.55)); // 0-100 -> 0-255
                // Turn off exhale pump
                digitalWrite(ctrlOutPump, LOW);
                // Close solenoid
                digitalWrite(ctrlSol, LOW);
                break;

            // Not used
            case pause:
                // Turn off inhale pump
                digitalWrite(ctrlHBrInA, LOW);
                digitalWrite(ctrlHBrInB, LOW);
                digitalWrite(ctrlHBrSel, LOW);
                digitalWrite(ctrlHBrEn, LOW);
                analogWrite(pwmInPump, 0);
                // Turn on exhale pump
                digitalWrite(ctrlOutPump, HIGH);
                // Open solenoid
                digitalWrite(ctrlSol, HIGH);
                break;

            case exhaleOn:
                // Turn off inhale pump
                digitalWrite(ctrlHBrInA, LOW);
                digitalWrite(ctrlHBrInB, LOW);
                digitalWrite(ctrlHBrSel, LOW);
                digitalWrite(ctrlHBrEn, LOW);
                analogWrite(pwmInPump, 0);
                // Turn on exhale pump
                digitalWrite(ctrlOutPump, HIGH);
                // Open solenoid
                digitalWrite(ctrlSol, HIGH);
                break;

            case exhaleOff:
                // Turn off inhale pump
                digitalWrite(ctrlHBrInA, LOW);
                digitalWrite(ctrlHBrInB, LOW);
                digitalWrite(ctrlHBrSel, LOW);
                digitalWrite(ctrlHBrEn, LOW);
                analogWrite(pwmInPump, 0); // 0-100 -> 0-255
                // Turn off exhale pump
                digitalWrite(ctrlOutPump, LOW);
                // Open solenoid
                digitalWrite(ctrlSol, HIGH);
                break;
        };
    }

#ifdef DEBUG
    delay(100);
#endif
}

void sendData() {

    char comState[2];
    char comCounter[6];
    char comBuffer[8] = "%";
    
    dtostrf((int) enmState, 1, 0, comState);
    dtostrf((unsigned int) TCNT1, 5, 0, comCounter);
    strcat(comBuffer, comState);
    strcat(comBuffer, comCounter);

    LOG("State: ");
    LOGLN(comState);
    LOG("Counter: ");
    LOGLN(comCounter);
    LOG("Buffer: ");
    LOGLN(comBuffer);

    Serial1.write(comBuffer, 7);
}

// Recieve machine state and TIMER1 counter from Master
// Return True on successful receive
bool receiveData() {

    char comState[2];
    char comCounter[6];
    char comBuffer[7];
    unsigned int recCounter;
    int recState;

    // Wait for special character
    // Break if 100 ms timeout reached
    unsigned long start = millis();
    while ((char) Serial1.read() != '%') {
        if ((millis() - start) > 300) {
            LOGLN("Read timeout");
            return false;
        }
    }

    // Read data
    size_t bytes = Serial1.readBytes(comBuffer, 6);
    if ((int) bytes != 6) {
        // Incorrect amount of chars read
        LOG("Incorrect number of bytes read: ");
        LOGLN(bytes);
        return false;
    }
    comBuffer[6] = '\0';

    // Extract state
    comState[0] = comBuffer[0];
    comState[1] = '\0';

    // Extract counter
    for (int i = 0; i <= 4; i++) {
        comCounter[i] = comBuffer[i + 1];
    }
    comCounter[5] = '\0';

    recCounter = atoi(comCounter);
    recState = atoi(comState);

    LOG("Buffer: ");
    LOGLN(comBuffer);
    LOG("State: ");
    LOGLN(recState);
    LOG("Counter: ");
    LOGLN(recCounter);

    // Verify sensible state value
    if (recState < 0 || recState > 4) {
        LOGLN("Invalid state");
        return false;
    }

    // Save master machine state and TIMER1 counter
    enmMasterState = (State) recState;
    TCNT1 = recCounter;

    return true;
}
