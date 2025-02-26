#pragma once

#include "Common.h"
#include <juce_dsp/juce_dsp.h>

/* just some methods for anayzing our
 * frequency domain transform data
 * */

namespace FFT {
/* Returns a vector containing the indeces of
 * every bin in the FFT data with a magnitude
 * in the given range
 * */
std::vector<size_t> getBinsInMagnitudeRange(float low,
                                            float high,
                                            std::complex<float>* data);
// Return all bin indices with a magnitude above the given value
std::vector<size_t> getBinsAboveMagnitude(float value,
                                          std::complex<float>* data);
// Return all bin indices with a magnitude below the given value
std::vector<size_t> getBinsBelowMagnitude(float value,
                                          std::complex<float>* data);
}  // namespace FFT
