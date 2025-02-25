#include "Phasermatic/PhaseProcessor.h"
#include "Phasermatic/Common.h"
#include "juce_core/juce_core.h"

void SineLFO::advance(int numSamples) {
  const float dPhase = twoPi_f / (float)SampleRate::get();
  phase = std::fmod(phase + (dPhase * (float)numSamples), twoPi_f);
  currentOutput = (std::sinf(phase) / 2.0f) + 0.5f;
}

//=====================================================================================

RandomOffsetsAlgo::RandomOffsetsAlgo() {
  juce::Random rand(2084);
  for (int i = 0; i < numBins; i++) {
    offsets[i] = rand.nextFloat() * twoPi_f;
  }
}

void RandomOffsetsAlgo::processBin(float*, float* phase, int bin, float depth) {
  *phase = flerp(*phase, *phase + offsets[bin], depth);
}

//=====================================================================================

PhaseProcessor::PhaseProcessor() {}

void PhaseProcessor::processBin(float* mag, float* phase, int bin) {
  *phase += randPhases1[bin];
}

void PhaseProcessor::processSpectrum(std::complex<float>* bins, int channel) {
  float magnitude, phase;
  for (int i = 0; i < numBins; i++) {
    magnitude = std::abs(bins[i]);
    phase = std::arg(bins[i]);
    // TODO: manipulate phase here
    bins[i] = std::polar(magnitude, phase);
  }
}

void PhaseProcessor::updateParams(apvts& tree) {
  currentType = (PhaserType)tree.getRawParameterValue("phasingType")->load();
  currentDepth = tree.getRawParameterValue("depth")->load();
  currentSpeed = tree.getRawParameterValue("lfoSpeed")->load();
  lfo.setSpeed(currentSpeed);
}

void PhaseProcessor::advanceSamples(int samples) {
  lfo.advance(samples);
}
