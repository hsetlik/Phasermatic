#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "juce_audio_processors/juce_audio_processors.h"

typedef juce::AudioProcessorValueTreeState apvts;
typedef juce::NormalisableRange<float> frange_t;
// maybe we'll avoid writing 5 zillion 'prepare()' functions this way
namespace SampleRate {
void set(double rate);
double get();
}  // namespace SampleRate
