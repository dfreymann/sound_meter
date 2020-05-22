/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/freymann/Dropbox/Electronics/_CODE/ParticleWorkbench/SoundMeter/src/SoundMeter.ino"
// 9.26.18
// dmf
// SoundMeter code

// 4.26.20 
// running on dmf_photon_9 (labeled "P 9")
// connected to foo_bar wifi network for the time being 
// updating for installation outside
// recompiling for latest OS

// 4.26.20 
// a) should have a handler for failure to upload (e.g. store values
// in an array and upload when have access)
// b) figure out a way to externalize wifi credentials that I can 
// work with.

// 5.22.20 
// i) the quick and dirty method of generating a running average of 
// the dBA output is (very) incorrect. writing a RunningAveragedB 
// library and will implement proper Leq average reporting.
// ii) also implement a count of # measurements 10dB above averagedB
// and # of measurements 10dB below averagedB in the time period
// iii) also implement a measure of 'ambient' - see references
// 
// 5.22.20 
// crap, found a bad bug - uint_8 (size parameter in the RunningAverage
// code) only accomodates to 256. I was passing it 200000, and silently
// being truncated so that only the last 32 values were being included
// in the moving average. If you try to allocate for uint_16, the program
// seems to just sit - I'm not going to try to debug this, a rolling 
// average (which requires allocating indexed positions) may not be needed. 
//
//
// ----------------------------------------

void setup();
void loop();
#line 36 "/Users/freymann/Dropbox/Electronics/_CODE/ParticleWorkbench/SoundMeter/src/SoundMeter.ino"
#define DEBUGS                               // 4.26.20 - use of "DEBUG" introduces conflict

// #include "application.h"                 // Particle Default (old) - 4.26.20 - had to comment this out 
#include "math.h"                           // For Some Reason...
// -----
#include "elapsedMillis.h"                  // Elapsed Timer
#include "RunningAveragedB.h"                 // Moving Average
#include "ThingSpeak.h"                     // Nice! Thingspeak has a library

// externalize API keys
#include "thingSpeak_tokens.h"

//
TCPClient client;

// Create the Timer
elapsedMillis measurementTime;
// and set the interval for output (or upload) in mS
uint16_t twentySeconds = 20000; // 20000; set to 2000 for testing

// Define a running average of the dB measurements
// ? define the count in terms of the output time, sort of...
int runningAvgCount = twentySeconds; // e.g. if 1 mS loop(), which
// actually may be ~ballpark for running in automatic mode (google)
RunningAveragedB runningAvgdB(twentySeconds);

// Define the voltage input pin
int dBVoltagePin = A0;
// Define the conversion from AnalogRead counts to dB
float dBAnalogConversion = 100 * (3.3 / 4096);
float maxdB = 0;

int icount = 0; 

// ----------------------------------------
void setup() {

  #ifdef DEBUGS
      // initialize Serial
      Serial.begin(115200);
      delay(100);
      Serial.println("... SoundMeter ...");
  #endif

  // Setup for Thingspeak
  ThingSpeak.begin(client);

  // initialize the running average to 0
  runningAvgdB.clear();

  // Do not need to declare the analog read pin:
    // "Note: you do not need to set the pinMode() with
    // analogRead(). The pinMode() is automatically set
    // to AN_INPUT any time analogRead() is called for
    // a particular analog pin"

  // initialize the timer
  measurementTime = 0;

}

// ----------------------------------------
void loop() {

  // read the Analog Input pin
  int dBVoltageValue = analogRead(dBVoltagePin);

  // note: dBVoltageValue will range from 0:4095 mapping
  // input voltages between 0 and 3.3 volts into
  // integer values between 0 and 4095.

  // Sound Meter output ranges from 0 - 1.1V max
  // (actually, ambient at ~40dB will be ~0.4V,
  // so the input range with this setup will be
  // ~0.4 - 1.1V, corresponding to integer values
  // of ~496 to ~1365)

  // The default ADC_SampleTime is 16 uS, and 5
  // samples are read with AnalogRead(), so readings
  // take 80uS

  // Convert integer VoltageValue to dB (10mV/dB)
  float dBMeasurement = dBVoltageValue * dBAnalogConversion;

  if (dBMeasurement > maxdB) maxdB = dBMeasurement;

  // Maintain a moving average of the sound level
  runningAvgdB.addValue(dBMeasurement);

  icount++;

  if (measurementTime > twentySeconds) {

    // When time to upload, generate the average
    float avgdB = runningAvgdB.getAverage();

    Serial.print(" number values sent ");
    Serial.println(icount); 
    icount = 0; 

    #ifdef DEBUGS
      Serial.print(" avgdB ");
      Serial.println(avgdB);
      Serial.print(" maxdB ");
      Serial.println(maxdB);
    #endif

    // prepare the information for upload [**** TO DO *****]
    // Note that the field values must be float (not unsigned long).
    ThingSpeak.setField(1,avgdB);
    ThingSpeak.setField(2,maxdB);

    // Write the fields all at once.
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    // reset timer
    measurementTime =  0;
    maxdB = 0;
  }
}

// ----------------------------------------
