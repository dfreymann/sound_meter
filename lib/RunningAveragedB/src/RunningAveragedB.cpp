// RunningAveragedB
// dmf 5.22.20 
// generate a 'correct' average of dB measurements 
// The running average is limited to uint_8 (256) 
// values, so is used only to maintain an ongoing 
// 'ambient' value in the SoundMeter.ino code
//
// Based on RunningAverage.cpp Verson 0.2.06 2015-mar-07
// by Rob Tillaart
// https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage

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
    for (uint16_t i = 0; i< _size; i++)
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

    _avg = _sum / _cnt; 
    if (_avg > 0) _logby10 = log10(_avg); 

    return (10 * _logby10);
}


// END OF FILE