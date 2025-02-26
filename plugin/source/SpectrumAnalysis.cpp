#include "Phasermatic/SpectrumAnalysis.h"

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
}  // namespace FFT
