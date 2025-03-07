#include "ReverbMachine/Common.h"

static double sampleHz = 44100.0;
static float fSampleHz = 44100.0f;

void SampleRate::set(double rate) {
  sampleHz = rate;
  fSampleHz = (float)rate;
}
double SampleRate::get() {
  return sampleHz;
}

float SampleRate::getf() {
  return fSampleHz;
}
