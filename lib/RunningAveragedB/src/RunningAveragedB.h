// RunningAveragedB
// dmf 5.22.20 
// generate a 'correct' average of dB measurements 
//

//
//    FILE: RunningAverage.h
//  AUTHOR: Rob dot Tillaart at gmail dot com
// VERSION: 0.2.06
//    DATE: 2015-mar-07
// PURPOSE: RunningAverage library for Arduino
//     URL: http://arduino.cc/playground/Main/RunningAverage
// HISTORY: See RunningAverage.cpp
//
// Released to the public domain
//
// backwards compatibility
// clr() clear()
// add(x) addValue(x)
// avg() getAverage()

#ifndef RunningAveragedB_h
#define RunningAveragedB_h

#define RUNNINGAVERAGEdB_LIB_VERSION "0.1"

//#include "Arduino.h"
//Arduino removed and Application added for spark.io compatibility
#include "application.h" 

// dmf 5.22.20 
class RunningAveragedB
{
public:
    RunningAveragedB(void);
    RunningAveragedB(uint16_t);
    ~RunningAveragedB();

    void clear();
    void addValue(double);

    double getAverage();

    uint16_t getSize() { return _size; }
    uint16_t getCount() { return _cnt; }

protected:
    uint16_t _size;
    uint16_t _cnt;
    uint16_t _idx;
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