#pragma once
#include "Common.h"

/** The main class that will do the work of moving phases around.
 * The data for effect algorithms and updates from the UI should
 * all be handled here
 *
 */
#pragma once

#define FFT_ORDER 10

class PhaseProcessor {
private:
  static constexpr int fftSize = 1 << FFT_ORDER;   // 1024 samples
  static constexpr int numBins = fftSize / 2 + 1;  // 513 bins
public:
  PhaseProcessor();
  void processBin(float* magnitude, float* phase, int bin);
};
