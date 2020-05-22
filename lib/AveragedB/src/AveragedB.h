// AveragedB
// dmf 5.22.20 
// generate a 'correct' simple average of dB measurements 
//

#ifndef AveragedB_h
#define AveragedB_h

#include "application.h" 

// dmf 5.22.20 
class AveragedB
{
public:
    AveragedB(void);
    AveragedB(uint16_t);
    ~AveragedB();

    void clear();
    void addValue(double);

    double getAverage();

    uint16_t getCount() { return _cnt; }

protected:
    uint16_t _cnt;
    double   _sum;

    // dmf 5.22.20 
    double _by10;
    double _expby10;
    double _avg;
    double _logby10; 

};

#endif
// END OF FILE