#include "ReverbMachine/GUI/LayoutHelpers.h"

namespace Layout {
std::vector<rect_f> makeGrid(juce::Component& comp, int rows, int columns) {
  auto fullBounds = comp.getLocalBounds().toFloat();
  const float dX = fullBounds.getWidth() / (float)columns;
  const float dY = fullBounds.getHeight() / (float)rows;
  std::vector<rect_f> vec;
  for (int r = 0; r < rows; r++) {
    const float y = dY * (float)r;
    for (int c = 0; c < columns; ++c) {
      const float x = dX * (float)c;
      vec.push_back({x, y, dX, dY});
    }
  }
  return vec;
}
}  // namespace Layout
