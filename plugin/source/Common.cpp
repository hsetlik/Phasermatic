#include "Phasermatic/Common.h"

// this way we only store this once (I hope)
static double sampleRate = 44100;

void SampleRate::set(double value) {
  sampleRate = value;
}
double SampleRate::get() {
  return sampleRate;
}
