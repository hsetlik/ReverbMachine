#include "ReverbMachine/PluginEditor.h"
#include "ReverbMachine/PluginProcessor.h"
#include "juce_core/juce_core.h"

namespace audio_plugin {
ReverbMachineProcessorEditor::ReverbMachineProcessorEditor(
    ReverbMachineAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p), spv(p.tree) {
  juce::ignoreUnused(processorRef);
  addAndMakeVisible(spv);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(800, 600);
}

ReverbMachineProcessorEditor::~ReverbMachineProcessorEditor() {}

void ReverbMachineProcessorEditor::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
}

void ReverbMachineProcessorEditor::resized() {
  spv.setBounds(getLocalBounds());
}
}  // namespace audio_plugin
