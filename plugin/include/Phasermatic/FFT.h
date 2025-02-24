#pragma once

#include "Common.h"
#include <juce_dsp/juce_dsp.h>

#define FFT_ORDER 10
typedef std::unique_ptr<juce::dsp::FFT> fft_core_t;
typedef juce::dsp::WindowingFunction<float> window_func_t;

// main core FFT processor

class FFTProcessor {
private:
  static constexpr int fftSize = 1 << FFT_ORDER;   // 1024 samples
  static constexpr int numBins = fftSize / 2 + 1;  // 513 bins
  static constexpr int overlap = 4;
  static constexpr int hopSize = fftSize / overlap;  // 256 samples
  // Gain correction for using Hann window with 75% overlap.
  static constexpr float windowCorrection = 2.0f / 3.0f;
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

public:
  FFTProcessor();
  // do the processing one sample at a time
  float processSample(float input);
};
