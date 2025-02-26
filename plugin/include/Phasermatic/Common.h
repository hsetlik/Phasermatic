#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "juce_audio_processors/juce_audio_processors.h"
#include "Identifiers.h"

// typedefs bc some of these types are a mouthful
typedef juce::AudioProcessorValueTreeState apvts;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
    slider_attach_ptr;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
    combo_attach_ptr;

// Keep the master list of algorithm types here
enum PhaserType { RandomOffsets, LFOFlip, ModulateOdds };
// return a list of the user-facing names for the above
juce::StringArray getEffectTypeNames();

constexpr float twoPi_f = juce::MathConstants<float>::twoPi;
// maybe we'll avoid writing 5 zillion 'prepare()' functions this way
namespace SampleRate {
void set(double rate);
double get();
}  // namespace SampleRate

inline float flerp(float a, float b, float t) {
  return a + ((b - a) * t);
}
