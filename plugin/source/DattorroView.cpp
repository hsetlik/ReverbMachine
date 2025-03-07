#include "ReverbMachine/GUI/DattorroView.h"
#include "ReverbMachine/GUI/LayoutHelpers.h"
#include "ReverbMachine/Identifiers.h"
#include "juce_core/juce_core.h"

DattorroView::DattorroView(apvts& tree)
    : preDelaySlider(tree, ID::DTRO_preDelay.toString()),
      preFilterSlider(tree, ID::DTRO_preFilter.toString()),
      id1Slider(tree, ID::DTRO_inDiff1.toString()),
      id2Slider(tree, ID::DTRO_inDiff2.toString()),
      decayDiffSlider(tree, ID::DTRO_decayDiff.toString()),
      dampingSlider(tree, ID::DTRO_damping.toString()),
      decaySlider(tree, ID::DTRO_decay.toString()) {
  for (int i = 0; i < 7; i++) {
    // sliders[i]->setSliderStyle(juce::Slider::LinearVertical);
    // sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 45, 15);
    addAndMakeVisible(sliders[i]);
  }
}

void DattorroView::resized() {
  auto grid = Layout::makeGrid(*this, 2, 4);
  for (int i = 0; i < 7; ++i) {
    sliders[i]->setBounds(grid[(size_t)i].toNearestInt());
  }
}

void DattorroView::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
}
