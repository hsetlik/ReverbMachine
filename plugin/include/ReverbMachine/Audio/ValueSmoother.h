#pragma once

#include "../Common.h"

// basic one-pole IIR filter for
// smoothing out values from the GUI

class ValueSmoother {
private:
  float coef = 0.6f;
  float outputVal = 0.0f;

public:
  ValueSmoother() = default;
  ValueSmoother(float smoothAmt) : coef(std::clamp(smoothAmt, 0.0f, 1.0f)) {}
  void setSmoothingFactor(float val) { coef = std::clamp(val, 0.0f, 1.0f); }
  float getSmoothingFactor() const { return coef; }

  float smooth(float input) {
    outputVal = ((1.0f - coef) * input) + (coef * outputVal);
    return outputVal;
  }
};
