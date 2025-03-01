#pragma once

#include "PluginProcessor.h"

namespace audio_plugin {

class ReverbMachineProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit ReverbMachineProcessorEditor(ReverbMachineAudioProcessor&);
  ~ReverbMachineProcessorEditor() override;
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  ReverbMachineAudioProcessor& processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbMachineProcessorEditor)
};

}  // namespace audio_plugin
