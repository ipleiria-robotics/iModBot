#ifndef E32_PC1_h
#define E32_PC1_h

#include "Arduino.h"
#include "driver/pcnt.h"

extern int16_t getCount();

class E32_PC1
{
    public:
        void clearCount();
        int16_t getCount();
        void begin(byte);
    private:
        //static void pcnt_init(void);

};

#endif