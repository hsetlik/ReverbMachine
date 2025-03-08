#include "ReverbMachine/GUI/SwitchingParentView.h"
#include "ReverbMachine/Identifiers.h"

SwitchingParentView::SwitchingParentView(apvts& tree)
    : typeBox(tree, ID::ReverbMode.toString()),
      wetDrySlider(tree, ID::wetDry.toString()),
      dattorro(tree),
      dattorroIIR(tree),
      spaceMod(tree) {
  // add all the views
  for (size_t i = 0; i < views.size(); ++i) {
    addAndMakeVisible(views[i]);
    if (i > 0) {
      views[i]->setVisible(false);
      views[i]->setEnabled(false);
    }
  }
  typeBox.addItemList(getReverbNames(), 1);
  typeBox.addListener(this);
  typeBox.setSelectedItemIndex(0);
  addAndMakeVisible(&typeBox);
  addAndMakeVisible(&wetDrySlider);
}

void SwitchingParentView::comboBoxChanged(juce::ComboBox* cb) {
  int newIdx = cb->getSelectedItemIndex();

  if (newIdx != activeViewIdx) {
    views[(size_t)activeViewIdx]->setEnabled(false);
    views[(size_t)activeViewIdx]->setVisible(false);
    activeViewIdx = newIdx;
    views[(size_t)activeViewIdx]->setVisible(true);
    views[(size_t)activeViewIdx]->setEnabled(true);
    views[(size_t)activeViewIdx]->toFront(false);
  }
  resized();
}

void SwitchingParentView::resized() {
  auto fBounds = getLocalBounds().toFloat();
  auto topBounds = fBounds.removeFromTop(20.0f);
  typeBox.setBounds(topBounds.toNearestInt());
  views[(size_t)activeViewIdx]->setBounds(fBounds.toNearestInt());
  const float dX = fBounds.getWidth() / 4.0f;
  const float dY = fBounds.getHeight() / 2.0f;
  rect_f wetDryBox = {3.0f * dX, topBounds.getHeight() + dY, dX, dY};
  wetDrySlider.setBounds(wetDryBox.toNearestInt());
  wetDrySlider.setEnabled(true);
  wetDrySlider.toFront(false);
}
