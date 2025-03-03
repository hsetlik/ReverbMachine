#include "ReverbMachine/GUI/ParamSlider.h"
#include <memory>

ParamSlider::ParamSlider() : aPtr(nullptr) {}

ParamSlider::ParamSlider(apvts& state, const juce::String& paramID)
    : aPtr(std::make_unique<apvts::SliderAttachment>(state, paramID, *this)) {}

void ParamSlider::atttachToState(apvts& state, const juce::String& paramID) {
  aPtr.reset(new apvts::SliderAttachment(state, paramID, *this));
}

//=========================================================
//
ParamComboBox::ParamComboBox() : aPtr(nullptr) {}

ParamComboBox::ParamComboBox(apvts& state, const juce::String& paramID)
    : aPtr(std::make_unique<apvts::ComboBoxAttachment>(state, paramID, *this)) {
}

void ParamComboBox::atttachToState(apvts& state, const juce::String& paramID) {
  aPtr.reset(new apvts::ComboBoxAttachment(state, paramID, *this));
}
