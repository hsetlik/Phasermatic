#include "Phasermatic/Common.h"

juce::StringArray getEffectTypeNames() {
  juce::StringArray arr;
  arr.add("Random Offsets");
  arr.add("Phase Flipper");
  arr.add("Modulate Odd");
  return arr;
}

std::complex<float> c_flerp(std::complex<float> a,
                            std::complex<float> b,
                            float t) {
  const float real = flerp(a.real(), b.real(), t);
  const float imag = flerp(a.imag(), b.imag(), t);
  return std::complex<float>(real, imag);
}

// this way we only store this once (I hope)
static double sampleRate = 44100;

void SampleRate::set(double value) {
  sampleRate = value;
}
double SampleRate::get() {
  return sampleRate;
}

//=======================================================================
//
namespace FFT {

static std::vector<float> getBinFrequecies() {
  std::vector<float> vec;
  for (int i = 0; i < numBins; ++i) {
    vec.push_back(getHzForBin(i));
  }
  return vec;
}
const std::vector<float> binFreqs = getBinFrequecies();

int getBinForHz(float hz) {
  // binary search. sinple
  size_t left = 0;
  size_t right = (size_t)numBins - 1;
  while (left <= right) {
    size_t mid = left + ((right - left) / 2);
    if (binFreqs[mid] <= hz && hz < binFreqs[mid + 1]) {
      return ((hz - binFreqs[mid]) > (binFreqs[mid + 1] - hz)) ? (int)mid + 1
                                                               : (int)mid;
    } else if (binFreqs[mid] < hz) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return 0;
}
}  // namespace FFT
