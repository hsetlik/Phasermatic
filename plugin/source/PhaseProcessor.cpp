#include "Phasermatic/PhaseProcessor.h"
#include "juce_core/juce_core.h"

PhaseProcessor::PhaseProcessor() {
  juce::Random rand(128032);
  for (int i = 0; i < numBins; i++) {
    randPhases1[i] = rand.nextFloat() * juce::MathConstants<float>::twoPi;
  }
}

void PhaseProcessor::processBin(float* mag, float* phase, int bin) {
  *phase += randPhases1[bin];
}
