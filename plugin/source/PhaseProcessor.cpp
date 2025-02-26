#include "Phasermatic/PhaseProcessor.h"
#include "Phasermatic/Common.h"
#include "juce_core/juce_core.h"

void SineLFO::advance(int numSamples) {
  const float dPhase = twoPi_f / (float)SampleRate::get();
  phase = std::fmod(phase + (dPhase * (float)numSamples * hz), twoPi_f);
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

//----------------------------------------------------------------

LFOFlipAlgo::LFOFlipAlgo(SineLFO* l) : lfo(l) {}

void LFOFlipAlgo::processBin(float* magnitude,
                             float* phase,
                             int bin,
                             float depth) {
  juce::ignoreUnused(magnitude, bin);
  // set the phase somewhere between itself and its inverse here
  *phase = flerp(*phase, *phase * -1.0f, lfo->getLevel() * depth);
}

//----------------------------------------------------------------

ModulateOddAlgo::ModulateOddAlgo(SineLFO* l) : lfo(l) {}

void ModulateOddAlgo::processBin(float* magnitude,
                                 float* phase,
                                 int bin,
                                 float depth) {
  // attenuate odd number frequency bins based on the LFO
  if (bin % 2) {
    float mod = depth * lfo->getLevel();
    *magnitude = (*magnitude * mod);
    *phase = (*phase * mod);
  }
}

//=====================================================================================

PhaseProcessor::PhaseProcessor() {
  // initialize each algorithm in the OwnedArray
  algos.add(new RandomOffsetsAlgo());
  algos.add(new LFOFlipAlgo(&lfo));
  algos.add(new ModulateOddAlgo(&lfo));
}

void PhaseProcessor::processSpectrum(std::complex<float>* bins, int channel) {
  float magnitude, phase;
  for (int i = 0; i < numBins; i++) {
    magnitude = std::abs(bins[i]);
    phase = std::arg(bins[i]);
    if (currentType < algos.size()) {
      algos[currentType]->processBin(&magnitude, &phase, i, currentDepth);
    }
    bins[i] = std::polar(magnitude, phase);
  }
}

void PhaseProcessor::updateParams(apvts& tree) {
  currentType = (int)tree.getRawParameterValue("phasingType")->load();
  currentDepth = tree.getRawParameterValue("depth")->load();
  currentSpeed = tree.getRawParameterValue("lfoSpeed")->load();
  lfo.setSpeed(currentSpeed);
}

void PhaseProcessor::advanceSamples(int samples) {
  lfo.advance(samples);
}
