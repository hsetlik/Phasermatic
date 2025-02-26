#pragma once

#include "../FFT.h"
#include "Phasermatic/PhaseProcessor.h"
#include "juce_graphics/juce_graphics.h"
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
