#pragma once
#include "Common.h"
#include <juce_dsp/juce_dsp.h>

/** The main class that will do the work of moving phases around.
 * The data for effect algorithms and updates from the UI should
 * all be handled here
 *
 */
#pragma once

// a basic sinewave LFO
class SineLFO {
private:
  float phase;
  float hz;
  float currentOutput;

public:
  SineLFO() = default;
  void setSpeed(float speed) { hz = speed; }
  void advance(int numSamples);
  float getLevel() const { return currentOutput; }
};

//========================================================================

// Each 'PhaserType' above has a corresponding PhaserAlgo to do the work
class PhaserAlgo {
public:
  PhaserAlgo() = default;
  virtual ~PhaserAlgo() = default;
  // this gets overridden by each child class
  virtual void processBin(float* magnitude,
                          float* phase,
                          int bin,
                          float depth) = 0;
};

class RandomOffsetsAlgo : public PhaserAlgo {
private:
  float offsets[numBins];

public:
  RandomOffsetsAlgo();
  void processBin(float* magnitude,
                  float* phase,
                  int bin,
                  float depth) override;
};

class LFOFlipAlgo : public PhaserAlgo {
private:
  SineLFO* const lfo;

public:
  LFOFlipAlgo(SineLFO* l);
  void processBin(float* magnitude,
                  float* phase,
                  int bin,
                  float depth) override;
};

class ModulateOddAlgo : public PhaserAlgo {
private:
  SineLFO* const lfo;

public:
  ModulateOddAlgo(SineLFO* l);
  void processBin(float* magnitude,
                  float* phase,
                  int bin,
                  float depth) override;
};
//==============================================================

//==============================================================
class PhaseProcessor {
private:
  // set of random bin values to use
  float randPhases1[numBins];

  // we'll update these values from the GUI once per audio buffer
  int currentType;
  float currentSpeed;
  float currentDepth;

  SineLFO lfo;

  juce::OwnedArray<PhaserAlgo> algos;

public:
  PhaseProcessor();
  // call this at the beginning of processBlock()
  void updateParams(apvts& tree);
  // process one fft hop worth of data
  void processSpectrum(std::complex<float>* buf, int channel = 0);
  // advance the LFOs and such
  void advanceSamples(int samples);
};
