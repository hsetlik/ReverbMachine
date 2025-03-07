#pragma once
#include "DattorroView.h"
#include "DattorroIIRView.h"
#include "LayoutHelpers.h"
#include "ReverbMachine/GUI/ParamSlider.h"
#include "juce_gui_basics/juce_gui_basics.h"

class SwitchingParentView : public juce::Component, juce::ComboBox::Listener {
private:
  ParamComboBox typeBox;
  LabeledParamSlider wetDrySlider;
  DattorroView dattorro;
  DattorroIIRView dattorroIIR;
  std::vector<juce::Component*> views = {&dattorro, &dattorroIIR};
  int activeViewIdx = 0;

public:
  SwitchingParentView(apvts& tree);
  void comboBoxChanged(juce::ComboBox* cb) override;
  void resized() override;
};
