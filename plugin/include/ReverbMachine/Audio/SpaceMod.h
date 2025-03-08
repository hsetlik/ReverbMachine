#pragma once

#include "DelayLine.h"
#include "ReverbBase.h"
#include "ReverbMachine/Audio/IIR.h"
#include "SineLFO.h"

class SpaceMod : public ReverbBase {
  // delay lines and filters---------------------------
  DelayLine preDelay;
  SingleIIR inputLowPass;
  SingleIIR inDiffIIRs[4];

  // left and right tanks
  DelayLine tankDiff1[2];
  DelayLine preFilterDelay[2];
  SingleIIR colorIIR1[2];
  SingleIIR colorIIR2[2];
  DelayLine postFilterDelay[2];
  DelayLine tankDiff2[2];

  // cycle tracking
  uint16_t t = 0;

  // parameters---------------------------------------
  const float preDelayAmt = 0.125f;
  const float tankDiff1Amt = 0.7f;
  const float tankDiff2Amt = 0.625f;

  // UI-accessible stuff
  float inFilterAmt;
  float lfoHz;
  float lfoTexture;
  float tankFilter1;
  float tankFilter2;
  float decay;
  float wetDry;
  // helpers that make up the processChunk method
  void processInput(float input);
  float getLeft();
  float getRight();

public:
  SpaceMod();
  void init(double sampleRate) override;
  void updateParams(apvts& tree) override;
  void processChunk(float* lBuf,
                    float* rBuf,
                    int numSamples,
                    bool iStereo,
                    bool oStereo) override;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpaceMod)
};
