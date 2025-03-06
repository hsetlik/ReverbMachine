#pragma once

#include "ParamSlider.h"
#include "../Common.h"
#include "juce_gui_basics/juce_gui_basics.h"

class DattorroIIRView : public juce::Component {
private:
  ParamSlider preDelaySlider;
  ParamSlider preFilterSlider;
  ParamSlider id1Slider;
  ParamSlider id2Slider;
  ParamSlider decayDiffSlider;
  ParamSlider dampingSlider;
  ParamSlider decaySlider;

  ParamSlider* sliders[7] = {
      &preDelaySlider,  &preFilterSlider, &id1Slider,   &id2Slider,
      &decayDiffSlider, &dampingSlider,   &decaySlider,
  };

public:
  DattorroIIRView(apvts& tree);
  void resized() override;
  void paint(juce::Graphics& g) override;
};
