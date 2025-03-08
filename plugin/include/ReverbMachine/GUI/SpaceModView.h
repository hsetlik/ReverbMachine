#pragma once

#include "ParamSlider.h"
#include "juce_gui_basics/juce_gui_basics.h"

class SpaceModView : public juce::Component {
private:
  LabeledParamSlider inFilterSlider;
  LabeledParamSlider lfoHzSlider;
  LabeledParamSlider lfoTexSlider;
  LabeledParamSlider inDiffSlider;
  LabeledParamSlider filter1Slider;
  LabeledParamSlider filter2Slider;
  LabeledParamSlider decaySlider;

  LabeledParamSlider* sliders[7] = {
      &inFilterSlider, &lfoHzSlider,   &lfoTexSlider, &inDiffSlider,
      &filter1Slider,  &filter2Slider, &decaySlider,
  };

public:
  SpaceModView(apvts& tree);
  void resized() override;
  void paint(juce::Graphics& g) override;
};
