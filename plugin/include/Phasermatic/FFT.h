#pragma once

#include "Common.h"
#include <juce_dsp/juce_dsp.h>
#include "PhaseProcessor.h"
#include "juce_core/system/juce_PlatformDefs.h"

typedef std::unique_ptr<juce::dsp::FFT> fft_core_t;
typedef juce::dsp::WindowingFunction<float> window_func_t;

// main core FFT processor

class FFTProcessor {
private:
  static constexpr int overlap = 4;
  static constexpr int hopSize = fftSize / overlap;  // 256 samples
  // Gain correction for using Hann window with 75% overlap.
  static constexpr float windowCorrection = 2.0f / 3.0f;
  PhaseProcessor* phaseProc;
  fft_core_t core;
  window_func_t window;
  // double buffering and FIFO stuff
  int samplePos = 0;
  float fftBuf[fftSize * 2];
  float aBuf[fftSize];
  float bBuf[fftSize];
  float* inPtr;
  float* outPtr;
  // other state stuff
  int hopCount = 0;

  // helpers
  void inputFull();
  void processFreqDomain();

public:
  FFTProcessor(PhaseProcessor* phaser);
  // do the processing one sample at a time
  float processSample(float input);
  // get a pointer to the frequency domain FFT data
  float* getFFTPointer() { return fftBuf; }
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFTProcessor)
};
