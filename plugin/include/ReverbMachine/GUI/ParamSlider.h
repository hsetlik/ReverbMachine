#pragma once
#include "ReverbMachine/Identifiers.h"
#include <juce_gui_basics/juce_gui_basics.h>

/* Slider subclass that manages its own
 * attachment to an apvts parameter
 * */
class ParamSlider : public juce::Slider {
private:
  slider_attach_ptr aPtr;

public:
  ParamSlider();
  ParamSlider(apvts& state, const juce::String& paramID);
  void atttachToState(apvts& state, const juce::String& paramID);
};

// same idea with a comboBox
class ParamComboBox : public juce::ComboBox {
private:
  combo_attach_ptr aPtr;

public:
  ParamComboBox();
  ParamComboBox(apvts& state, const juce::String& paramID);
  void atttachToState(apvts& state, const juce::String& paramID);
};
