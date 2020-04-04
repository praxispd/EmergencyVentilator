#ifndef CONTROL_H
#define CONTROL_H


class Control{
    public:

    Control(int pin, int minRead=0,int maxRead=1023, float minOut=0, float maxOut=100);
    
    void setMaxRead(int value);
    void setMinRead(int value);
    void setMinOut(int value);
    void setMaxOut(int value);

    float read(int pin);
    float avgRead(int pin, int reads);

    protected:
        int minRead;
        int maxRead;
        int minOut;
        int maxOut;


};

#endif