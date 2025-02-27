#include "Phasermatic/SpectrumAnalysis.h"
#include "Phasermatic/Common.h"

namespace FFT {
std::vector<size_t> getBinsInMagnitudeRange(float low,
                                            float high,
                                            std::complex<float>* data) {
  std::vector<size_t> bins;
  for (size_t b = 0; b < numBins; b++) {
    const float mag = std::abs(data[b]);
    if (mag < high && mag >= low) {
      bins.push_back(b);
    }
  }
  return bins;
}

std::vector<size_t> getBinsAboveMagnitude(float value,
                                          std::complex<float>* data) {
  return getBinsInMagnitudeRange(value, 1.0f, data);
}

std::vector<size_t> getBinsBelowMagnitude(float value,
                                          std::complex<float>* data) {
  return getBinsInMagnitudeRange(0.0f, value, data);
}

float getMagnitudeAtHz(float hz, std::complex<float>* data) {
  size_t left = 0;
  size_t right = (size_t)numBins - 1;
  while (left <= right) {
    size_t mid = left + ((right - left) / 2);
    if (binFreqs()[mid] <= hz && hz < binFreqs()[mid + 1]) {
      auto& low = binFreqs()[mid];
      auto& high = binFreqs()[mid + 1];
      const float t = (hz - low) / (high - low);
      const float magLow = std::abs(data[mid]);
      const float magHigh = std::abs(data[mid + 1]);
      return flerp(magLow, magHigh, t);
    } else if (binFreqs()[mid] < hz) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return 0.0f;
}

float meanMagnitudeInBand(float startHz,
                          float endHz,
                          std::complex<float>* data) {
  int bin = getBinForHz(startHz);
  const int endBin = getBinForHz(endHz);
  float sum = 0.0f;
  float denom = 0.0f;
  while (bin <= endBin) {
    sum += std::abs(data[bin]);
    denom += 1.0f;
  }
  return sum / denom;
}
}  // namespace FFT
