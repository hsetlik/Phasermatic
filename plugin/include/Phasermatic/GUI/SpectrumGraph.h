#pragma once

#include "../FFT.h"
#include "Phasermatic/PhaseProcessor.h"
#include "juce_graphics/juce_graphics.h"
#include "../SpectrumAnalysis.h"
#define GRAPH_REFRESH_HZ 24

class FFTGraph : public juce::Component {
private:
  // state/updating stuff
  bool wantsUpdate = false;
  juce::ScrollBar sb;
  // geometry/layout stuff
  static constexpr int totalHeight = 276;
  static constexpr int width = 600;
  static constexpr int scrollBarHeight = 15;
  static constexpr int imageHeight = totalHeight - scrollBarHeight;
  // buffer to hold our FFT data
  std::complex<float> fftData[numBins];
  // the bitmap for the graph
  juce::Image img;

public:
  FFTGraph();
  void paint(juce::Graphics& g) override;
  // call this in processBlock(), it limits its own refresh rate
  void refresh(float* data);
  void resized() override;

private:
  // helper for the heavy lifting
  void updateImageData(std::complex<float>* fft);
};

//===========================================================

class SevenBandGraph : public juce::Component {
private:
  static constexpr int imgHeight = 265;
  static constexpr int imgWidth = 600;
  std::complex<float> bins[numBins];
  float band[7];
  juce::Image img;

public:
  SevenBandGraph();
  void refresh(float* data);
  void paint(juce::Graphics& g) override;

private:
  void updateImageData(std::complex<float>* fft);
};

//===========================================================

#define GRAPH_BANDS 10

class MultibandGraph : public juce::Component {
private:
  static constexpr int imgHeight = 265;
  static constexpr int imgWidth = 600;
  std::complex<float> bins[numBins];
  float band[GRAPH_BANDS];
  juce::Image img;

public:
  MultibandGraph();
  void refresh(float* data);
  void paint(juce::Graphics& g) override;

private:
  void updateImageData(std::complex<float>* fft);
};
