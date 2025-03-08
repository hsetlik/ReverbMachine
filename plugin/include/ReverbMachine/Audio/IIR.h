#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <memory.h>
#include "ValueSmoother.h"

// type aliases for brevity
typedef juce::dsp::IIR::Filter<float> iir_core_t;
typedef juce::dsp::IIR::Coefficients<float> iir_coeffs_t;
typedef juce::dsp::FilterDesign<float> cascade_d;

// these are the types of single (biquad)
// IIR filter, from which our more complex
// higher-order filters are built
enum iir_type_t {
  FirstOrderLowPass,
  FirstOrderHighPass,
  FirstOrderAllPass,
  NormalLowPass,
  NormalHighPass,
  NormalBandPass,
  NormalNotch,
  NormalAllPass,
  LowPassQ,
  HighPassQ,
  BandPassQ,
  NotchQ,
  AllPassQ,
  LowShelf,
  HighShelf,
  Peak
};

#define NUM_IIR_TYPES 16

struct single_iir_params_t {
  uint8_t filterType = iir_type_t::NormalLowPass;
  float frequency = 2000.0f;
  float q = 0.0f;
  float gain = 1.0f;
  single_iir_params_t& operator=(const single_iir_params_t& other) {
    filterType = other.filterType;
    frequency = other.frequency;
    q = other.q;
    gain = other.gain;
    return *this;
  }
  single_iir_params_t() = default;
  single_iir_params_t(const single_iir_params_t& other) {
    filterType = other.filterType;
    frequency = other.frequency;
    q = other.q;
    gain = other.gain;
  }
  bool operator==(single_iir_params_t& other) {
    bool tComp = filterType == other.filterType;
    bool fComp = fequal(frequency, other.frequency);
    bool qComp = fequal(q, other.q);
    bool gComp = fequal(gain, other.gain);
    return tComp && fComp && qComp && gComp;
  }
};

// wrapper around juce's IIR biquad implementation
class SingleIIR {
private:
  bool useFreqSmoothing = false;
  // value smoothing stuff
  bool smoothingActive = false;
  ValueSmoother smoother;
  float targetFreq = 2000.0f;
  float smoothedFreq = 2000.0f;

  //--------------------
  std::unique_ptr<iir_core_t> core;
  single_iir_params_t params;
  double sampleRate;
  bool filterPrepared = false;
  // helper for re-setting up the unique_ptr
  single_iir_params_t lastPrepareParams;
  void prepareFilter();

public:
  SingleIIR();
  void setParams(single_iir_params_t other);
  single_iir_params_t* getParams() { return &params; }
  void setFrequency(float freq);
  void prepare(double sampleFreq);
  float process(float input);
  void setFreqSmoothing(bool useSmoothing) { useFreqSmoothing = useSmoothing; }
};

// enum for the different IIR filter topologies
// provided in the dsp::FilterDesign juce module
// https://docs.juce.com/master/structdsp_1_1FilterDesign.html

enum iir_cascade_t {
  ButterworthLowPass1,
  ButterworthLowPass2,
  ButterworthHighPass,
  Chebyshev1LowPass,
  Chebyshev2LowPass,
  EllipticLowPass
};

#define NUM_CASCADE_TYPES 6

struct cascade_iir_params_t {
  uint8_t filterType = 0;
  int order = 4;
  float cutoff = 2000.0f;
  float transitionWidth = 0.5f;
  float passbandGain = 1.0;
  float stopbandGain = 0.5f;
  cascade_iir_params_t() {}
  cascade_iir_params_t& operator=(const cascade_iir_params_t& other) {
    filterType = other.filterType;
    order = other.order;
    cutoff = other.cutoff;
    transitionWidth = other.transitionWidth;
    passbandGain = other.passbandGain;
    stopbandGain = other.stopbandGain;
    return *this;
  }
  cascade_iir_params_t(const cascade_iir_params_t& other) {
    filterType = other.filterType;
    order = other.order;
    cutoff = other.cutoff;
    transitionWidth = other.transitionWidth;
    passbandGain = other.passbandGain;
    stopbandGain = other.stopbandGain;
  }
};

class CascadeIIR {
private:
  juce::OwnedArray<iir_core_t> filters;
  cascade_iir_params_t params;
  double sampleRate = 44100.0f;
  bool filtersPrepared = false;
  // same idea as prepareFilter() above
  void prepareCascade();

public:
  CascadeIIR() = default;
  void setParams(cascade_iir_params_t other);
  void prepare(double sampleFreq);
  float process(float input);
};
