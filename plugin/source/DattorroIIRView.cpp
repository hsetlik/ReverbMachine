#include "ReverbMachine/GUI/DattorroIIRView.h"
#include "ReverbMachine/Identifiers.h"
#include "juce_core/juce_core.h"

DattorroIIRView::DattorroIIRView(apvts& tree)
    : preDelaySlider(tree, ID::DTRI_preDelay.toString()),
      preFilterSlider(tree, ID::DTRI_preFilter.toString()),
      id1Slider(tree, ID::DTRI_lowPass.toString()),
      id2Slider(tree, ID::DTRI_lowPass.toString()),
      decayDiffSlider(tree, ID::DTRI_width.toString()),
      dampingSlider(tree, ID::DTRI_damping.toString()),
      decaySlider(tree, ID::DTRI_decay.toString()) {
  for (int i = 0; i < 7; i++) {
    sliders[i]->setSliderStyle(juce::Slider::LinearVertical);
    sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 45, 15);
    addAndMakeVisible(sliders[i]);
  }
}

void DattorroIIRView::resized() {
  auto lowerBounds = getLocalBounds().toFloat();
  const float dY = lowerBounds.getHeight() / 2.0f;
  auto upperBounds = lowerBounds.removeFromTop(dY);
  const float dX = upperBounds.getWidth() / 4.0f;
  rect_f t1 = {0.0f, 0.0f, dX, dY - 12.0f};
  rect_f t2 = {dX, 0.0f, dX, dY - 12.0f};
  rect_f t3 = {2.0f * dX, 0.0f, dX, dY - 12.0f};
  rect_f t4 = {3.0f * dX, 0.0f, dX, dY - 12.0f};

  rect_f b1 = {0.0f, dY, dX, dY - 12.0f};
  rect_f b2 = {dX, dY, dX, dY - 12.0f};
  rect_f b3 = {2.0f * dX, dY, dX, dY - 12.0f};

  preDelaySlider.setBounds(t1.toNearestInt());
  preFilterSlider.setBounds(t2.toNearestInt());
  id1Slider.setBounds(t3.toNearestInt());
  id2Slider.setBounds(t4.toNearestInt());
  decayDiffSlider.setBounds(b1.toNearestInt());
  dampingSlider.setBounds(b2.toNearestInt());
  decaySlider.setBounds(b3.toNearestInt());
}

void DattorroIIRView::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
}
