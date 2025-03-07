#include "ReverbMachine/GUI/DattorroIIRView.h"
#include "ReverbMachine/GUI/LayoutHelpers.h"
#include "ReverbMachine/Identifiers.h"
#include "juce_core/juce_core.h"

DattorroIIRView::DattorroIIRView(apvts& tree)
    : preDelaySlider(tree, ID::DTRI_preDelay.toString()),
      preFilterSlider(tree, ID::DTRI_preFilter.toString()),
      id1Slider(tree, ID::DTRI_lowPass.toString()),
      id2Slider(tree, ID::DTRI_highPass.toString()),
      decayDiffSlider(tree, ID::DTRI_width.toString()),
      dampingSlider(tree, ID::DTRI_damping.toString()),
      decaySlider(tree, ID::DTRI_decay.toString()) {
  for (int i = 0; i < 7; i++) {
    // sliders[i]->setSliderStyle(juce::Slider::LinearVertical);
    // sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 45, 15);
    addAndMakeVisible(sliders[i]);
  }
}

void DattorroIIRView::resized() {
  auto grid = Layout::makeGrid(*this, 2, 4);
  for (int i = 0; i < 7; ++i) {
    sliders[i]->setBounds(grid[(size_t)i].toNearestInt());
  }
}

void DattorroIIRView::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
}
