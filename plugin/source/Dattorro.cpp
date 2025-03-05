#include "ReverbMachine/Audio/Dattorro.h"
#include "ReverbMachine/Identifiers.h"
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
Dattorro::Dattorro() {}

void Dattorro::init(double sampleRate) {
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
  decayAmt = 0.75f;
  decayDiff1Amt = 0.70f;
  dampingAmt = 0.95f;
  wetDry = 0.85f;
}

void Dattorro::updateParams(apvts& tree) {
  // grab values in thread-safe way
  const float _preDelay =
      tree.getRawParameterValue(ID::DTRO_preDelay.toString())->load();
  const float _preFilter =
      tree.getRawParameterValue(ID::DTRO_preFilter.toString())->load();
  const float _inDiff1 =
      tree.getRawParameterValue(ID::DTRO_inDiff1.toString())->load();
  const float _inDiff2 =
      tree.getRawParameterValue(ID::DTRO_inDiff2.toString())->load();
  const float _dDiff =
      tree.getRawParameterValue(ID::DTRO_decayDiff.toString())->load();
  const float _damping =
      tree.getRawParameterValue(ID::DTRO_damping.toString())->load();
  const float _decay =
      tree.getRawParameterValue(ID::DTRO_decay.toString())->load();

  preFilterAmt = _preFilter;
  preDelayAmt = _preDelay;
  uint16_t _pdSamples = (uint16_t)(preDelayAmt * (float)maxPreDelay);
  preDelay.setDelay(0, _pdSamples);
  inputDiff1Amt = _inDiff1;
  inputDiff2Amt = _inDiff2;
  decayDiff1Amt = _dDiff;
  decayDiff2Amt = std::clamp(_decay + 0.15f, 0.25f, 0.5f);
  dampingAmt = _damping;
  decayAmt = _decay;
}

void Dattorro::processChunk(float* lBuf,
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

void Dattorro::processInput(float input) {
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
  static float _preFilter = 0.0f;
  x = LP_process(&_preFilter, x, preFilterAmt);

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
    x1 = LP_process(&damping[i], x1, dampingAmt);
    x1 *= decayAmt;
    x1 = decayDiffusion2[i].processDiffuser(t, x1, decayDiff2Amt);
    postDampingDelay[i].write(t, x1);
  }
  // increment t
  t++;
}

float Dattorro::getLeft() {
  float a = preDampingDelay[1].read(1, t);
  a += preDampingDelay[1].read(2, t);
  a -= decayDiffusion2[1].read(2, t);
  a += postDampingDelay[1].read(2, t);
  a -= preDampingDelay[0].read(3, t);
  a -= decayDiffusion2[0].read(1, t);
  a += postDampingDelay[0].read(1, t);
  return a;
}

float Dattorro::getRight() {
  float a = preDampingDelay[0].read(1, t);
  a += preDampingDelay[0].read(2, t);
  a -= decayDiffusion2[0].read(2, t);
  a += postDampingDelay[0].read(2, t);
  a -= preDampingDelay[1].read(3, t);
  a -= decayDiffusion2[1].read(1, t);
  a += postDampingDelay[1].read(1, t);
  return a;
}
