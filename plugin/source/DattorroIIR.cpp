#include "ReverbMachine/Audio/DattorroIIR.h"
#include "ReverbMachine/Audio/IIR.h"
#include "ReverbMachine/Identifiers.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"

// filter helpers
// this guy is like the simplest possible low pass filter
// it just lerps the input with the previous output where
// the freq argument is the proportion of the previous output
static float LP_process(float* out, float in, float freq) {
  *out += (in - *out) * freq;
  return *out;
}

//================================================================
DattorroIIR::DattorroIIR() {}

void DattorroIIR::init(double sampleRate) {
  // the max predelay should be
  // about 1/10th the sample rate
  maxPreDelay = (uint16_t)(sampleRate / 10.0);
  // Initialize all the delay lines with Dattorro's 'magic numbers'
  preDelay.init(maxPreDelay);
  // input diffusion delays
  inDiffusion[0].init(142);
  inDiffusion[1].init(107);
  inDiffusion[2].init(379);
  inDiffusion[3].init(277);
  // left tank----------------------------
  decayDiffusion1[0].init(672);

  preDampingDelay[0].init(4453);
  preDampingDelay[0].setDelay(1, 353);
  preDampingDelay[0].setDelay(2, 3627);
  preDampingDelay[0].setDelay(3, 1990);

  decayDiffusion2[0].init(1800);
  decayDiffusion2[0].setDelay(1, 187);
  decayDiffusion2[0].setDelay(2, 1228);

  postDampingDelay[0].init(3720);
  postDampingDelay[0].setDelay(1, 1066);
  postDampingDelay[0].setDelay(2, 2673);

  // right tank----------------------------
  decayDiffusion1[1].init(908);

  preDampingDelay[1].init(4217);
  preDampingDelay[1].setDelay(1, 266);
  preDampingDelay[1].setDelay(2, 2974);
  preDampingDelay[1].setDelay(3, 2111);

  decayDiffusion2[1].init(2656);
  decayDiffusion2[1].setDelay(1, 335);
  decayDiffusion2[1].setDelay(2, 1913);

  postDampingDelay[1].init(3163);
  postDampingDelay[1].setDelay(1, 121);
  postDampingDelay[1].setDelay(2, 1996);

  // default param values-------------------
  preDelay.setDelay(0, maxPreDelay / 10);
  preFilterAmt = 0.85f;
  inputDiff1Amt = 0.75f;
  inputDiff2Amt = 0.625f;
  hiPassHz = 70.0f;
  loPassHz = 1400.0f;

  decayAmt = 0.75f;
  decayDiff1Amt = 0.70f;
  dampingAmt = 0.95f;
  wetDry = 0.85f;
  preFilterIIR.setFreqSmoothing(true);
  preFilterIIR.prepare(sampleRate);
  setPreFilter(preFilterAmt);
  // set up the two filters for each tank
  for (int i = 0; i < 2; i++) {
    // set up high pass
    auto hpPar = *hiPass[i].getParams();
    hpPar.filterType = iir_type_t::NormalHighPass;
    hpPar.frequency = hiPassHz;
    hiPass[i].setFreqSmoothing(true);
    hiPass[i].setParams(hpPar);
    hiPass[i].prepare(sampleRate);
    // set up low pass
    auto lpPar = *loPass[i].getParams();
    lpPar.frequency = loPassHz;
    lpPar.filterType = iir_type_t::NormalLowPass;
    loPass[i].setFreqSmoothing(true);
    loPass[i].setParams(lpPar);
    loPass[i].prepare(sampleRate);
  }
}

void DattorroIIR::updateParams(apvts& tree) {
  // grab values in thread-safe way
  const float _preDelay =
      tree.getRawParameterValue(ID::DTRI_preDelay.toString())->load();
  const float _preFilter =
      tree.getRawParameterValue(ID::DTRI_preFilter.toString())->load();
  const float _lp =
      tree.getRawParameterValue(ID::DTRI_lowPass.toString())->load();
  const float _hp =
      tree.getRawParameterValue(ID::DTRI_highPass.toString())->load();
  const float _width =
      tree.getRawParameterValue(ID::DTRI_width.toString())->load();
  const float _damping =
      tree.getRawParameterValue(ID::DTRI_damping.toString())->load();
  const float _decay =
      tree.getRawParameterValue(ID::DTRI_decay.toString())->load();
  const float _wetDry =
      tree.getRawParameterValue(ID::wetDry.toString())->load();

  preFilterAmt = _preFilter;
  preDelayAmt = _preDelay;
  uint16_t _pdSamples = (uint16_t)(preDelayAmt * (float)maxPreDelay);
  preDelay.setDelay(0, _pdSamples);
  decayDiff2Amt = std::clamp(_decay + 0.15f, 0.25f, 0.5f);
  dampingAmt = _damping;
  decayAmt = _decay;
  wetDry = _wetDry;

  // set up the pre-filter lowpass
  setPreFilter(preFilterAmt);
  setupTankFilters(_lp, _hp, _width);
}

void DattorroIIR::setPreFilter(float amt) {
  static frange_t pfRange = rangeWithCenter(500.0f, 3500.0f, 1500.0f);
  float hz = pfRange.convertFrom0to1(1.0f - amt);
  preFilterIIR.setFrequency(hz);
}

void DattorroIIR::setupTankFilters(float lpHz, float hpHz, float width) {
  static const float hpGainMin = juce::Decibels::decibelsToGain(-5.3f);
  static const float hpGainMax = juce::Decibels::decibelsToGain(3.4f);
  static const float lpGainMin = juce::Decibels::decibelsToGain(-5.3f);
  static const float lpGainMax = juce::Decibels::decibelsToGain(3.4f);

  static float maxHpCoeffs[2] = {hpGainMin, hpGainMax};
  static float maxLpCoeffs[2] = {lpGainMin, lpGainMax};
  // if any of these three change we have to recalculate all of them
  if (!fequal(lpHz, loPassHz) || !fequal(hpHz, hiPassHz) ||
      !fequal(width, stereoWidth)) {
    loPassHz = lpHz;
    hiPassHz = hpHz;
    stereoWidth = width;
    // now set the actual cutoff for each filter based on the
    // stereo width
    for (int i = 0; i < 2; ++i) {
      float lFreq = loPassHz * flerp(1.0f, maxLpCoeffs[i], stereoWidth);
      float hFreq = hiPassHz * flerp(1.0f, maxHpCoeffs[i], stereoWidth);
      loPass[i].setFrequency(lFreq);
      hiPass[i].setFrequency(hFreq);
    }
  }
}

void DattorroIIR::processChunk(float* lBuf,
                               float* rBuf,
                               int numSamples,
                               bool iStereo,
                               bool oStereo) {
  if (!oStereo) {
    for (int i = 0; i < numSamples; ++i) {
      processInput(lBuf[i]);
      lBuf[i] = (getLeft() + getRight()) / 2.0f;
    }
  } else if (!iStereo) {  // mono in / Stereo out
    for (int i = 0; i < numSamples; ++i) {
      processInput(lBuf[i]);
      lBuf[i] = getLeft();
      rBuf[i] = getRight();
    }
  } else {  // stereo / stereo
    float mix;
    for (int i = 0; i < numSamples; ++i) {
      mix = (lBuf[i] + rBuf[i]) / 2.0f;
      processInput(mix);
      lBuf[i] = getLeft();
      rBuf[i] = getRight();
    }
  }
}

//===========================================================================================

void DattorroIIR::processInput(float input) {
  // modulate decayDiffusion1 for both tanks
  //
  // this is decimal 2047
  // the bitwise and means that the if statement
  // will run once per 2048 samples, a total of 32
  // times per the 65536 samples in our 16 bit cycle
  // In practice this this equates to a triangle wave LFO
  // with a frequency of sampleRate/65536 hz
  // For a standard 44.1 kHz sample rate that's about 0.67 hz
  if ((t & 0x07ff) == 0) {
    if (t < (1 << 15)) {
      decayDiffusion1[0].offsets[0]--;
      decayDiffusion1[1].offsets[0]--;
    } else {
      decayDiffusion1[0].offsets[0]++;
      decayDiffusion1[1].offsets[0]++;
    }
  }
  // pre-delay
  float x = preDelay.process(t, input);
  // pre-filter
  x = preFilterIIR.process(x);
  // input diffusion
  x = inDiffusion[0].processDiffuser(t, x, inputDiff1Amt);
  x = inDiffusion[1].processDiffuser(t, x, inputDiff1Amt);
  x = inDiffusion[2].processDiffuser(t, x, inputDiff2Amt);
  x = inDiffusion[3].processDiffuser(t, x, inputDiff2Amt);

  // process the tanks
  float x1;
  for (uint16_t i = 0; i < 2; i++) {
    // cross feedback
    x1 = x + postDampingDelay[1 - i].read(0, t) * decayAmt;
    // back to this half ot the tank
    x1 = decayDiffusion1[i].processDiffuser(t, x1, decayDiff1Amt);
    x1 = preDampingDelay[i].process(t, x1);
    x1 = loPass[i].process(x1);
    x1 *= decayAmt;
    x1 = decayDiffusion2[i].processDiffuser(t, x1, decayDiff2Amt);
    postDampingDelay[i].write(t, x1);
  }
  // increment t
  t++;
}

float DattorroIIR::getLeft() {
  float a = preDampingDelay[1].read(1, t);
  a += preDampingDelay[1].read(2, t);
  a -= decayDiffusion2[1].read(2, t);
  a += postDampingDelay[1].read(2, t);
  a -= preDampingDelay[0].read(3, t);
  a -= decayDiffusion2[0].read(1, t);
  a += postDampingDelay[0].read(1, t);
  const float dryVal = preDelay.read(0, t);
  return flerp(dryVal, a, wetDry);
}

float DattorroIIR::getRight() {
  float a = preDampingDelay[0].read(1, t);
  a += preDampingDelay[0].read(2, t);
  a -= decayDiffusion2[0].read(2, t);
  a += postDampingDelay[0].read(2, t);
  a -= preDampingDelay[1].read(3, t);
  a -= decayDiffusion2[1].read(1, t);
  a += postDampingDelay[1].read(1, t);

  const float dryVal = preDelay.read(0, t);
  return flerp(dryVal, a, wetDry);
}
