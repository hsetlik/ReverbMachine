#include "ReverbMachine/Identifiers.h"
#include <memory>
#include "juce_audio_processors/juce_audio_processors.h"

frange_t rangeWithCenter(float start, float end, float center) {
  frange_t range(start, end);
  range.setSkewForCentre(center);
  return range;
}

juce::StringArray getReverbNames() {
  juce::StringArray arr;
  arr.add("Dattorro");
  arr.add("Dattorro IIR");
  return arr;
}

apvts::ParameterLayout ID::getParameterLayout() {
  apvts::ParameterLayout layout;
  // Commmon----------------------------------------
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      ID::ReverbMode.toString(), "Reverb mode", getReverbNames(), 0));
  const frange_t wetDryRange = rangeWithCenter(0.0f, 1.0f, 0.5f);
  const float wetDryDefault = 0.8f;
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::wetDry.toString(), "Wet/dry mix", wetDryRange, wetDryDefault));

  // Dattorro---------------------------------------
  const frange_t preDelayRange = rangeWithCenter(0.0f, 1.0f, 0.35f);
  const float preDelayDefault = 0.1f;
  const frange_t preFilterRange = rangeWithCenter(0.0f, 1.0f, 0.75f);
  const float preFilterDefault = 0.85f;
  const frange_t iDiff1Range = rangeWithCenter(0.0f, 1.0f, 0.6f);
  const float iDiff1Default = 0.75f;
  const frange_t iDiff2Range = rangeWithCenter(0.0f, 1.0f, 0.6f);
  const float iDiff2Default = 0.625f;
  const frange_t decayRange = rangeWithCenter(0.0f, 1.0f, 0.6f);
  const float decayDefault = 0.75f;
  const frange_t dDiffRange = rangeWithCenter(0.0f, 1.0f, 0.7f);
  const float dDiffDefault = 0.7f;
  const frange_t dampingRange = rangeWithCenter(0.0f, 1.0f, 0.85f);
  const float dampingDefault = 0.95f;

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRO_preDelay.toString(), "Pre-delay", preDelayRange,
      preDelayDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRO_preFilter.toString(), "Pre-filter", preFilterRange,
      preFilterDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRO_inDiff1.toString(), "In diff. 1", iDiff1Range, iDiff1Default));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRO_inDiff2.toString(), "In diff. 2", iDiff2Range, iDiff2Default));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRO_decayDiff.toString(), "Decay diff.", dDiffRange, dDiffDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRO_decay.toString(), "Decay", decayRange, decayDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRO_damping.toString(), "Damping", dampingRange, dampingDefault));

  // DattorroIIR---------------------------------------------

  const frange_t hpRange = rangeWithCenter(45.0f, 2000.0f, 85.0f);
  const float hpDefault = 70.0f;
  const frange_t lpRange = rangeWithCenter(700.0f, 6500.0f, 1200.0f);
  const float lpDefault = 1400.0f;

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRI_preDelay.toString(), "Pre-delay", preDelayRange,
      preDelayDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRI_preFilter.toString(), "Pre-filter", preFilterRange,
      preFilterDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRI_highPass.toString(), "High Pass", hpRange, hpDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRI_lowPass.toString(), "Low Pass", lpRange, lpDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRI_width.toString(), "Width", dDiffRange, dDiffDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRI_decay.toString(), "Decay", decayRange, decayDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::DTRI_damping.toString(), "Damping", dampingRange, dampingDefault));
  return layout;
}
