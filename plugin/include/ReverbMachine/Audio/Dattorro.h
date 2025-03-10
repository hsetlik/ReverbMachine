#pragma once
#include "DelayLine.h"
#include "ReverbBase.h"

class Dattorro : public ReverbBase {
private:
  // Delay lines & state------------------------------
  DelayLine preDelay;
  DelayLine inDiffusion[4];
  uint16_t maxPreDelay;
  // the left and right 'tanks'
  DelayLine decayDiffusion1[2];
  DelayLine decayDiffusion2[2];
  DelayLine preDampingDelay[2];
  DelayLine postDampingDelay[2];
  float damping[2];
  // for tracking our sample offset
  uint16_t t = 0;
  // Parameters------------------------------
  float preDelayAmt;
  float preFilterAmt;
  float inputDiff1Amt;
  float inputDiff2Amt;
  float decayDiff1Amt;
  float decayDiff2Amt;
  float dampingAmt;
  float decayAmt;
  float wetDry;

public:
  Dattorro();
  void init(double sampleRate) override;
  void updateParams(apvts& tree) override;
  void processChunk(float* lBuf,
                    float* rBuf,
                    int numSamples,
                    bool iStereo,
                    bool oStereo) override;

private:
  // both the process methods above are made out of these helpers
  void processInput(float input);
  float getLeft();
  float getRight();
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dattorro)
};
