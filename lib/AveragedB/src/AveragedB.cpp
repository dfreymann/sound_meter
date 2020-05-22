// AveragedB
// dmf 5.22.20 
// generate a 'correct' simple average of dB measurements 
//

#include "AveragedB.h"
#include <stdlib.h>
#include <math.h>

AveragedB::AveragedB()
{
    clear();
}

// resets all counters
void AveragedB::clear()
{
    _cnt = 0;
    _sum = 0.0;
    _logby10 = 0.0; 
}

// adds a new value to the data-set
void AveragedB::addValue(double value)
{
    _by10 = value/10;
    _expby10 = pow(10, _by10); 

    _sum += _expby10;
    _cnt++;

}

// returns the average of the data-set added sofar
double AveragedB::getAverage()
{
    if (_cnt == 0) return 0;

    _avgpow = _sum / _cnt; 
    if (_avgpow > 0) _logby10 = log10(_avgpow); 
    _avg = 10 * _logby10; 
   
    clear(); 

    return (_avg);
}


// END OF FILE