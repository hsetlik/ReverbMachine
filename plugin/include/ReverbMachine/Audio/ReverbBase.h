#pragma once

#include "../Common.h"

// the abstract class which our various reverbs inherit
class ReverbBase {
public:
  ReverbBase()=default;
  virtual ~ReverbBase()=default;

  virtual void init(double sampleRate)=0;
  virtual void updateParams(apvts& tree)=0;
  virtual void processChunk(float* lBuf, float* rBuf, int numSamples, bool inStereo, bool outStereo)=0;
};
