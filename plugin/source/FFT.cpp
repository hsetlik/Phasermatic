#include "Phasermatic/FFT.h"

FFTProcessor::FFTProcessor(PhaseProcessor* phaser)
    : phaseProc(phaser),
      core(std::make_unique<juce::dsp::FFT>(FFT_ORDER)),
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
  samplePos = (samplePos + 1) % fftSize;
  ++hopCount;
  if (hopCount == hopSize) {
    inputFull();
    hopCount = 0;
  }
  return output;
}

void FFTProcessor::inputFull() {
  // 1. copy the relevant chunk of the input buffer into the real part of our
  // FFT buffer
  std::memcpy(fftBuf, inPtr + samplePos, (fftSize - samplePos) * sizeof(float));
  if (samplePos > 0) {
    std::memcpy(fftBuf + fftSize - samplePos, inPtr, samplePos * sizeof(float));
  }

  // 2. apply the windowing function here
  window.multiplyWithWindowingTable(fftBuf, fftSize);

  // 3. do the FFT
  core->performRealOnlyForwardTransform(fftBuf, true);
  processFreqDomain();
  core->performRealOnlyInverseTransform(fftBuf);

  // 4. apply the windowing function again
  window.multiplyWithWindowingTable(fftBuf, fftSize);

  // 5. copy to the output buffer
  for (int i = 0; i < samplePos; i++) {
    outPtr[i] += fftBuf[i + fftSize - samplePos] * windowCorrection;
  }
  for (int i = 0; i < fftSize - samplePos; i++) {
    outPtr[i + samplePos] += fftBuf[i] * windowCorrection;
  }
}

void FFTProcessor::processFreqDomain() {
  // the FFT buffer is interleaved real/imaginary
  // so we can easily cast it to std::complex;
  auto* bins = reinterpret_cast<std::complex<float>*>(fftBuf);
  float magnitude, phase;
  for (int i = 0; i < numBins; i++) {
    magnitude = std::abs(bins[i]);
    phase = std::arg(bins[i]);
    phaseProc->processBin(&magnitude, &phase, i);
    bins[i] = std::polar(magnitude, phase);
  }
}
