#include "ReverbMachine/GUI/SpaceModView.h"
#include "ReverbMachine/GUI/LayoutHelpers.h"
#include "ReverbMachine/Identifiers.h"

SpaceModView::SpaceModView(apvts& tree)
    : inFilterSlider(tree, ID::SPMD_inFilter.toString()),
      lfoHzSlider(tree, ID::SPMD_lfoHz.toString()),
      lfoTexSlider(tree, ID::SPMD_lfoTexture.toString()),
      inDiffSlider(tree, ID::SPMD_inDiff.toString()),
      filter1Slider(tree, ID::SPMD_tankFilter1.toString()),
      filter2Slider(tree, ID::SPMD_tankFilter2.toString()),
      decaySlider(tree, ID::SPMD_decay.toString()) {
  for (int i = 0; i < 7; ++i) {
    addAndMakeVisible(sliders[i]);
  }
}

void SpaceModView::resized() {
  auto grid = Layout::makeGrid(*this, 2, 4);
  for (int i = 0; i < 7; ++i) {
    sliders[i]->setBounds(grid[(size_t)i].toNearestInt());
  }
}

void SpaceModView::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
}
