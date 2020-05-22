// RunningAveragedB
// dmf 5.22.20 
// generate a 'correct' average of dB measurements 
//

//
//    FILE: RunningAverage.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.06
//    DATE: 2015-mar-07
// PURPOSE: RunningAverage library for Arduino
//
// The library stores N individual values in a circular buffer,
// to calculate the running average.
//
// HISTORY:
// 0.1.00 - 2011-01-30 initial version
// 0.1.01 - 2011-02-28 fixed missing destructor in .h
// 0.2.00 - 2012-??-?? Yuval Naveh added trimValue (found on web)
//          http://stromputer.googlecode.com/svn-history/r74/trunk/Arduino/Libraries/RunningAverage/RunningAverage.cpp
// 0.2.01 - 2012-11-21 refactored
// 0.2.02 - 2012-12-30 refactored trimValue -> fillValue
// 0.2.03 - 2013-11-31 getElement
// 0.2.04 - 2014-07-03 added memory protection
// 0.2.05 - 2014-12-16 changed float -> double
// 0.2.06 - 2015-03-07 all size uint8_t
//
// Released to the public domain
//

#include "RunningAveragedB.h"
#include <stdlib.h>
#include <math.h>

RunningAveragedB::RunningAveragedB(uint8_t size)
{
    _size = size;
    _ar = (double*) malloc(_size * sizeof(double));
    if (_ar == NULL) _size = 0;
    clear();
}

RunningAveragedB::~RunningAveragedB()
{
    if (_ar != NULL) free(_ar);
}

// resets all counters
void RunningAveragedB::clear()
{
    _cnt = 0;
    _idx = 0;
    _sum = 0.0;
    for (uint8_t i = 0; i< _size; i++)
    {
        _ar[i] = 0.0; // keeps addValue simple
    }
}

// adds a new value to the data-set
void RunningAveragedB::addValue(double value)
{
    if (_ar == NULL) return;

    // dmf 5.22.20 
    _by10 = value/10;
    _expby10 = pow(10, _by10); 

    _sum -= _ar[_idx];
    // dmf 5.22.20 
    _ar[_idx] = _expby10;
    _sum += _ar[_idx];
    _idx++;
    if (_idx == _size) _idx = 0;  // faster than %
    if (_cnt < _size) _cnt++;
}

// returns the average of the data-set added sofar
double RunningAveragedB::getAverage()
{
    if (_cnt == 0) return 0;
    // dmf 5.22.20
    _avg = _sum / _cnt; 
    if (_avg > 0) _logby10 = log10(_avg); 
    return (10 * _logby10);
}


// END OF FILE