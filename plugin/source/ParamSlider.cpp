#include "ReverbMachine/GUI/ParamSlider.h"
#include <memory>
#include "juce_events/juce_events.h"

ParamSlider::ParamSlider() : aPtr(nullptr) {}

ParamSlider::ParamSlider(apvts& state, const juce::String& paramID)
    : aPtr(std::make_unique<apvts::SliderAttachment>(state, paramID, *this)) {}

void ParamSlider::atttachToState(apvts& state, const juce::String& paramID) {
  aPtr.reset(new apvts::SliderAttachment(state, paramID, *this));
}

//=========================================================

LabeledParamSlider::LabeledParamSlider(apvts& state,
                                       const juce::String& paramID)
    : slider(state, paramID) {
  slider.setSliderStyle(juce::Slider::LinearVertical);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 45, 15);
  addAndMakeVisible(slider);
  // grab the user-facing parameter name
  const juce::String text = state.getParameter(paramID)->name;
  label.setText(text, juce::dontSendNotification);
  label.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(label);
}

void LabeledParamSlider::resized() {
  auto fBounds = getLocalBounds().toFloat();
  constexpr float labelHeight = 18.0f;
  auto lBounds = fBounds.removeFromTop(labelHeight);
  label.setBounds(lBounds.toNearestInt());
  slider.setBounds(fBounds.toNearestInt());
}

//=========================================================
ParamComboBox::ParamComboBox() : aPtr(nullptr) {}

ParamComboBox::ParamComboBox(apvts& state, const juce::String& paramID)
    : aPtr(std::make_unique<apvts::ComboBoxAttachment>(state, paramID, *this)) {
}

void ParamComboBox::atttachToState(apvts& state, const juce::String& paramID) {
  aPtr.reset(new apvts::ComboBoxAttachment(state, paramID, *this));
}
