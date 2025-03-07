#pragma once
#include "ReverbMachine/Identifiers.h"
#include <juce_gui_basics/juce_gui_basics.h>

#define LABEL_HEIGHT_PX 12.0f

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

class LabeledParamSlider : public juce::Component {
private:
  ParamSlider slider;
  juce::Label label;

public:
  LabeledParamSlider(apvts& state, const juce::String& paramID);
  ParamSlider* getSlider() { return &slider; }
  void resized() override;
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
