#include "Phasermatic/GUI/SpectrumGraph.h"
#include "Phasermatic/Common.h"
#include "Phasermatic/PhaseProcessor.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_graphics/juce_graphics.h"

FFTGraph::FFTGraph()
    : sb(false), img(juce::Image::RGB, width, imageHeight, true) {
  sb.setRangeLimits({0.0, (double)numBins});
  sb.setCurrentRange(0.0, 30.0);
  addAndMakeVisible(&sb);
}

void FFTGraph::refresh(float* buf) {
  auto* complex = reinterpret_cast<std::complex<float>*>(buf);
  updateImageData(complex);
  repaint();
}

void FFTGraph::resized() {
  const int scrollY = getLocalBounds().getHeight() - scrollBarHeight;
  sb.setBounds(0, scrollY, width, scrollBarHeight);
}

void FFTGraph::paint(juce::Graphics& g) {
  static const juce::Rectangle<float> iBounds =
      getLocalBounds().removeFromTop(imageHeight).toFloat();
  g.drawImage(img, iBounds);
}

void FFTGraph::updateImageData(std::complex<float>* fft) {
  // 1. copy the data into out own buffer
  std::memcpy(fftData, fft, sizeof fftData);
  // 2. determine which bins we should draw
  auto range = sb.getCurrentRange();
  const int firstBin = (int)range.getStart();
  const int lastBin = (int)range.getEnd();
  // 3. figure out widths
  float fBinWidth = (float)width / (float)(lastBin - firstBin);
  const int barWidth = (int)(fBinWidth / 2.0f);
  const int binWidth = 2 * barWidth;
  // 4. Prepare the image buffer
  static const juce::Rectangle<int> imgBounds =
      getLocalBounds().removeFromTop(imageHeight);
  img.clear(imgBounds);
  // 5. draw the magnitude & phase bars
  static const juce::Colour magColor(218, 165, 32);
  static const juce::Colour phaseColor(144, 238, 144);
  int currentX = 0;
  int bin = firstBin;
  while (bin <= lastBin) {
    // grip the phase and magnitude values
    constexpr float minDb = -80.0f;
    float levelDb = std::max<float>(
        juce::Decibels::gainToDecibels<float>(std::abs(fftData[bin])), minDb);

    const float mag = 1.0f - (levelDb / minDb);
    const float phase = std::arg(fftData[bin]);
    const int yMag = imageHeight - (int)(mag * 255.0f);
    const int yPhase =
        imageHeight - (int)(std::abs<float>(phase / twoPi_f) * 255.0f);
    juce::Rectangle<int> mRect(currentX, yMag, barWidth, imageHeight - yMag);
    juce::Rectangle<int> pRect(currentX + barWidth, yPhase, barWidth,
                               imageHeight - yPhase);
    img.clear(mRect, magColor);
    img.clear(pRect, phaseColor);
    currentX += binWidth;
    bin++;
  }
}
