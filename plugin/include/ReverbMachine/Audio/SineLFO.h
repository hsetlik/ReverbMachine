#pragma once
#include "../Common.h"

#define NUM_SINE_VALUES 2048
#define MAX_TEXTURE 0.65f

class SineLFO {
protected:
  float currentHz = 0.5f;
  float phaseDelta = 0.0f;
  float phase = 0.0f;

public:
  SineLFO();
  float getSpeed() const { return currentHz; }
  void setSpeed(float hz);
  float get();
};

//=================================

class TripleSineLFO {
private:
  SineLFO subLFO;
  SineLFO centerLFO;
  SineLFO octLFO;
  float currentHz = 0.5f;
  float texture = 0.0f;

public:
  TripleSineLFO();
  float getSpeed() const { return currentHz; }
  void setSpeed(float hz);
  void setTexture(float tex) { texture = tex * MAX_TEXTURE; }
  float get();
};
