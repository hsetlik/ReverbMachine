#pragma once
/* This namespace is were all the string identifiers for
 * parameters, components, and anything else should live.
 * Simply use the `DECLARE_ID` macro and never worry about
 * misspelling a string literal again.
 *
 * This translation unit is also responsible for the
 * `ID::getParameterLayout()` function which we use to set up
 * the AudioProcessorValueTreeState that runs everything
 * */
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

// typedefs bc some of these types are a mouthful
typedef juce::AudioProcessorValueTreeState apvts;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
    slider_attach_ptr;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
    combo_attach_ptr;
typedef juce::NormalisableRange<float> frange_t;

frange_t rangeWithCenter(float start, float end, float center);

#define DECLARE_ID(name) const juce::Identifier name(#name);

// enum of all the reverb algorithms
enum ReverbType { Dattorro };

juce::StringArray getReverbNames();

namespace ID {
// top level ID for the apvts
DECLARE_ID(ReverbMachine_state)
// which reverb type to use
DECLARE_ID(ReverbMode)

// we can use the same wet/dry ID throughout
DECLARE_ID(wetDry)

// Dattorro params----------------------------------------
DECLARE_ID(DTRO_preDelay)
DECLARE_ID(DTRO_preFilter)
DECLARE_ID(DTRO_inDiff1)
DECLARE_ID(DTRO_inDiff2)
DECLARE_ID(DTRO_decayDiff)
DECLARE_ID(DTRO_damping)
DECLARE_ID(DTRO_decay)

//----------------------------------------

apvts::ParameterLayout getParameterLayout();
}  // namespace ID

#undef DECLARE_ID
