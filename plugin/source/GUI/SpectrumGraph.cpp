#include "Phasermatic/GUI/SpectrumGraph.h"
#include <cmath>
#include "Phasermatic/Common.h"
#include "Phasermatic/Identifiers.h"
#include "Phasermatic/PhaseProcessor.h"
#include "Phasermatic/SpectrumAnalysis.h"
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
//=====================================================================================
SevenBandGraph::SevenBandGraph()
    : img(juce::Image::RGB, imgWidth, imgHeight, true) {}

void SevenBandGraph::refresh(float* buf) {
  auto* complex = reinterpret_cast<std::complex<float>*>(buf);
  updateImageData(complex);
  repaint();
}

void SevenBandGraph::paint(juce::Graphics& g) {
  static juce::Rectangle<float> imgBounds = getLocalBounds().toFloat();
  g.drawImage(img, imgBounds);
}

static frange_t getGraphRange() {
  frange_t range(0.0f, 1.0f);
  const float center = juce::Decibels::decibelsToGain<float>(-17.0f);
  range.setSkewForCentre(center);
  return range;
}

void SevenBandGraph::updateImageData(std::complex<float>* ptr) {
  std::memcpy(bins, ptr, sizeof bins);
  // 1. update the band level values
  band[0] = FFT::meanMagnitudeInBand(20.0f, 80.0f, bins);
  band[1] = FFT::meanMagnitudeInBand(80.0f, 140.0f, bins);
  band[2] = FFT::meanMagnitudeInBand(150.0f, 315.0f, bins);
  band[3] = FFT::meanMagnitudeInBand(325.0f, 750.0f, bins);
  band[4] = FFT::meanMagnitudeInBand(765.0f, 2200.0f, bins);
  band[5] = FFT::meanMagnitudeInBand(2220.0f, 6000.0f, bins);
  band[6] = FFT::meanMagnitudeInBand(6050.0f, 16000.0f, bins);
  // 2. put those on a decibel scale
  static frange_t dbRange = getGraphRange();
  //  for (int i = 0; i < 7; ++i) {
  //   band[i] = dbRange.convertFrom0to1(band[i]);
  //}
  // 3. draw the image
  int currentX = 7;
  constexpr int barWidth = 588 / 7;
  static juce::Rectangle<int> imgBounds = getLocalBounds();
  img.clear(imgBounds);
  static const juce::Colour magColor(218, 165, 32);
  for (int i = 0; i < 7; ++i) {
    const int barHeight = (int)(band[i] * (float)imgHeight);
    const int barY = imgHeight - barHeight;
    juce::Rectangle<int> rect(currentX + 3, barY, barWidth - 3, barHeight);
    img.clear(rect, magColor);
    currentX += barWidth;
  }
}
//

//=====================================================================================
MultibandGraph::MultibandGraph()
    : img(juce::Image::RGB, imgWidth, imgHeight, true) {}

void MultibandGraph::refresh(float* buf) {
  auto* complex = reinterpret_cast<std::complex<float>*>(buf);
  updateImageData(complex);
  repaint();
}

void MultibandGraph::paint(juce::Graphics& g) {
  g.drawImageAt(img, 0, 0);
}

// some helpers for figuring out where frequency bands should be
static std::vector<freq_band_t> getExponentialFreqBands(int numBands) {
  constexpr float fMin = 50.0f;
  constexpr float fMax = 15000.0f;
  /*  Just a little algebra
   *  max = min * x^numBands
   *  max / min = x^numBands
   *  x = (max / min)^(1 / numBands)
   * */
  const float root = std::powf(fMax / fMin, 1.0f / (float)numBands);
  std::vector<freq_band_t> bands;
  for (int b = 0; b < numBands; b++) {
    const float start = fMin * std::powf(root, (float)b);
    const float next = fMin * std::powf(root, (float)b + 1.0f);
    bands.push_back({start, next - 10.0f});
  }
  return bands;
}

// TODO maybe have this be decibel-based in the future
static int barHeightforMagnitude(float mag, int maxHeight) {
  return (int)(mag * (float)maxHeight);
}

void MultibandGraph::updateImageData(std::complex<float>* fft) {
  static const std::vector<freq_band_t> ranges =
      getExponentialFreqBands(GRAPH_BANDS);
  // 1. find the magnitude for each band
  for (size_t b = 0; b < GRAPH_BANDS; b++) {
    band[b] =
        FFT::meanMagnitudeInBand(ranges[b].getStart(), ranges[b].getEnd(), fft);
  }
  // 2. do some geometry
  constexpr int xStart = 5;
  constexpr int xEnd = imgWidth - xStart;
  constexpr int barWidth = (xEnd - xStart) / GRAPH_BANDS;
  // 3. clear the image and draw the graph
  static juce::Rectangle<int> imgBounds = getLocalBounds();
  img.clear(imgBounds);
  static const juce::Colour magColor(218, 165, 32);
  for (int b = 0; b < GRAPH_BANDS; b++) {
    const int x = xStart + (barWidth * b);
    const int height = barHeightforMagnitude(band[b], imgHeight);
    juce::Rectangle<int> bar = {x, imgHeight - height, barWidth, height};
    img.clear(bar, magColor);
  }
}
