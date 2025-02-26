#include "Phasermatic/Common.h"

juce::StringArray getEffectTypeNames() {
  juce::StringArray arr;
  arr.add("Random Offsets");
  arr.add("Phase Flipper");
  return arr;
}

// this way we only store this once (I hope)
static double sampleRate = 44100;

void SampleRate::set(double value) {
  sampleRate = value;
}
double SampleRate::get() {
  return sampleRate;
}
