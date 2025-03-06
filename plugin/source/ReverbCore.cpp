#include "ReverbMachine/Audio/ReverbCore.h"
#include "ReverbMachine/Identifiers.h"

ReverbCore::ReverbCore() : core(nullptr) {
  prepareReverbCore(0);
}

void ReverbCore::prepareReverbCore(int type) {
  ReverbType id = (ReverbType)type;
  switch (id) {
    case ReverbType::Dattorro_t:
      core.reset(new Dattorro());
      break;
    case ReverbType::DattorroIIR_t:
      core.reset(new DattorroIIR());
      break;
    default:
      core.reset(new Dattorro());
      break;
  }
  core->init(SampleRate::get());
}

void ReverbCore::processChunk(float* lBuf,
                              float* rBuf,
                              int numSamples,
                              bool inStereo,
                              bool outStereo) {
  core->processChunk(lBuf, rBuf, numSamples, inStereo, outStereo);
}

void ReverbCore::updateParams(apvts& tree) {
  const int nType =
      (int)tree.getRawParameterValue(ID::ReverbMode.toString())->load();
  if (currentReverbType != nType) {
    currentReverbType = nType;
    prepareReverbCore(nType);
  }
  core->updateParams(tree);
}
