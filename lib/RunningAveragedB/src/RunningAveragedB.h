// RunningAveragedB
// dmf 5.22.20 
// generate a 'correct' average of dB measurements 
// The running average is limited to uint_8 (256) 
// values, so is used only to maintain an ongoing 
// 'ambient' value in the SoundMeter.ino code
//

#ifndef RunningAveragedB_h
#define RunningAveragedB_h

#include "application.h" 

// dmf 5.22.20 
class RunningAveragedB
{
public:
    RunningAveragedB(void);
    RunningAveragedB(uint8_t);
    ~RunningAveragedB();

    void clear();
    void addValue(double);

    double getAverage();

    uint8_t getSize() { return _size; }
    uint8_t getCount() { return _cnt; }

protected:
    uint8_t _size;
    uint8_t _cnt;
    uint8_t _idx;
    double   _sum;
    double * _ar;

    // dmf 5.22.20 
    double _by10;
    double _expby10;
    double _avg;
    double _logby10 = 0; 

};

#endif
// END OF FILE