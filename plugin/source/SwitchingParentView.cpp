#include "ReverbMachine/GUI/SwitchingParentView.h"
#include "ReverbMachine/Identifiers.h"

SwitchingParentView::SwitchingParentView(apvts& tree)
    : typeBox(tree, ID::ReverbMode.toString()),
      wetDrySlider(tree, ID::wetDry.toString()),
      dattorro(tree) {
  typeBox.addItemList(getReverbNames(), 1);
  typeBox.addListener(this);
  typeBox.setSelectedItemIndex(0);
  addAndMakeVisible(&typeBox);
  addAndMakeVisible(&wetDrySlider);
  wetDrySlider.setSliderStyle(juce::Slider::LinearVertical);
  wetDrySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 45, 15);
  addAndMakeVisible(&dattorro);
}

void SwitchingParentView::comboBoxChanged(juce::ComboBox* cb) {
  int newIdx = cb->getSelectedItemIndex();

  if (newIdx != activeViewIdx) {
    views[(size_t)activeViewIdx]->setEnabled(false);
    views[(size_t)activeViewIdx]->setVisible(false);
    activeViewIdx = newIdx;
    views[(size_t)activeViewIdx]->setVisible(true);
    views[(size_t)activeViewIdx]->setVisible(true);
  }
}

void SwitchingParentView::resized() {
  auto fBounds = getLocalBounds().toFloat();
  auto topBounds = fBounds.removeFromTop(20.0f);
  typeBox.setBounds(topBounds.toNearestInt());
  views[(size_t)activeViewIdx]->setBounds(fBounds.toNearestInt());
  const float dX = fBounds.getWidth() / 4.0f;
  const float dY = fBounds.getHeight() / 2.0f;
  rect_f wetDryBox = {3.0f * dX, dY + 12.0f, dX, dY - 12.0f};
  wetDrySlider.setBounds(wetDryBox.toNearestInt());
}
