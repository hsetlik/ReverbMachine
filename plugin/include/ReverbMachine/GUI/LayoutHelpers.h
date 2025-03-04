#pragma once

#include "../Common.h"
#include "ReverbMachine/Identifiers.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace Layout {
std::vector<rect_f> makeGrid(juce::Component& comp, int rows, int columns);
}
