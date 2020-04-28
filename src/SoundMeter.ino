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

//
//
// ----------------------------------------

#define DEBUGS                               // 4.26.20 - use of "DEBUG" introduces conflict

// #include "application.h"                 // Particle Default (old) - 4.26.20 - had to comment this out 
#include "math.h"                           // For Some Reason...
// -----
#include "elapsedMillis.h"                  // Elapsed Timer
#include "RunningAverage.h"                 // Moving Average
#include "ThingSpeak.h"                     // Nice! Thingspeak has a library

// externalize API keys
#include "thingSpeak_tokens.h"

//
TCPClient client;

// Create the Timer
elapsedMillis measurementTime;
// and set the interval for output (or upload) in mS
unsigned int twentySeconds = 20000; // 20000; set to 2000 for testing

// Define a running average of the dB measurements
// ? define the count in terms of the output time, sort of...
int runningAvgCount = twentySeconds; // e.g. if 1 mS loop(), which
// actually may be ~ballpark for running in automatic mode (google)
RunningAverage runningAvgdB(twentySeconds);

// Define the voltage input pin
int dBVoltagePin = A0;
// Define the conversion from AnalogRead counts to dB
float dBAnalogConversion = 100 * (3.3 / 4096);
float maxdB = 0;

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

  if (measurementTime > twentySeconds) {

    // When time to upload, generate the average
    float avgdB = runningAvgdB.getAverage();

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
