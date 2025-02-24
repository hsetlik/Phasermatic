#include "Phasermatic/FFT.h"

FFTProcessor::FFTProcessor()
    : core(std::make_unique<juce::dsp::FFT>(FFT_ORDER)),
      window(fftSize + 1,
             juce::dsp::WindowingFunction<float>::WindowingMethod::hann,
             false) {
  inPtr = aBuf;
  outPtr = bBuf;
}

float FFTProcessor::processSample(float input) {
  inPtr[samplePos] = input;
  float output = outPtr[samplePos];
  outPtr[samplePos] = 0.0f;
}

void FFTProcessor::inputFull() {}
