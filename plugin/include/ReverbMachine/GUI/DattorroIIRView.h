#pragma once

#include "ParamSlider.h"
#include "../Common.h"
#include "juce_gui_basics/juce_gui_basics.h"

class DattorroIIRView : public juce::Component {
private:
  LabeledParamSlider preDelaySlider;
  LabeledParamSlider preFilterSlider;
  LabeledParamSlider id1Slider;
  LabeledParamSlider id2Slider;
  LabeledParamSlider decayDiffSlider;
  LabeledParamSlider dampingSlider;
  LabeledParamSlider decaySlider;

  LabeledParamSlider* sliders[7] = {
      &preDelaySlider,  &preFilterSlider, &id1Slider,   &id2Slider,
      &decayDiffSlider, &dampingSlider,   &decaySlider,
  };

public:
  DattorroIIRView(apvts& tree);
  void resized() override;
  void paint(juce::Graphics& g) override;
};
