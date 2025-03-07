#include "ReverbMachine/Audio/SineLFO.h"
#include <cmath>
#include "ReverbMachine/Common.h"

static std::array<float, NUM_SINE_VALUES> s_getSineArray() {
  std::array<float, NUM_SINE_VALUES> arr;
  float phase;
  for (size_t i = 0; i < NUM_SINE_VALUES; ++i) {
    phase = ((float)i / (float)NUM_SINE_VALUES) * twoPi_f;
    float full = std::sin(phase);
    arr[i] = (full + 1.0f) * 0.5f;
  }
  return arr;
}

static std::array<float, NUM_SINE_VALUES> sineValues = s_getSineArray();

//==========================================

SineLFO::SineLFO() {
  setSpeed(currentHz);
}

void SineLFO::setSpeed(float hz) {
  if (!fequal(currentHz, hz)) {
    currentHz = hz;
    // delta in terms of phase 0-1
    phaseDelta = hz / SampleRate::getf();
  }
}

float SineLFO::get() {
  static const float fLen = (float)(NUM_SINE_VALUES - 1);
  phase = std::fmod(phase + phaseDelta, 1.0f);
  return sineValues[(size_t)(phase * fLen)];
}

//==========================================

TripleSineLFO::TripleSineLFO() {
  setSpeed(currentHz);
}

void TripleSineLFO::setSpeed(float hz) {
  if (!fequal(hz, currentHz)) {
    currentHz = hz;
    centerLFO.setSpeed(currentHz);
    subLFO.setSpeed(currentHz / 2.0f);
    octLFO.setSpeed(currentHz * 2.0f);
  }
}

float TripleSineLFO::get() {
  float sides = (subLFO.get() + octLFO.get()) * 0.5f;
  return flerp(centerLFO.get(), sides, texture);
}
