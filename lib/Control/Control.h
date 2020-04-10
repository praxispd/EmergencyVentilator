#ifndef CONTROL_H
#define CONTROL_H

class Control {
    public:

    Control(int pin, float minOut = 0, float maxOut = 100, int minRead = 0, int maxRead = 1023);
    
    void setMaxRead(int value);
    void setMinRead(int value);
    void setMinOut(int value);
    void setMaxOut(int value);

    float read();
    float avgRead(int reads, int delayTime);
    float safeRead();

    protected:

        int pin;
        float minOut;
        float maxOut;
        int minRead;
        int maxRead;
        float lastRead;
};

#endif