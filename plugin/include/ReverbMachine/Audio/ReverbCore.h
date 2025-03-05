#pragma once

#include "Dattorro.h"

class ReverbCore {
private:
  std::unique_ptr<ReverbBase> core;
  int currentReverbType = -1;
  void prepareReverbCore(int type);

public:
  ReverbCore();
  void updateParams(apvts& tree);

  void processChunk(float* lBuf,
                    float* rBuf,
                    int numSamples,
                    bool inStereo,
                    bool outStereo);
};
