#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "juce_audio_processors/juce_audio_processors.h"

// typedefs bc some of these types are a mouthful
typedef juce::AudioProcessorValueTreeState apvts;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
    slider_attach_ptr;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
    combo_attach_ptr;
typedef juce::NormalisableRange<float> frange_t;
// maybe we'll avoid writing 5 zillion 'prepare()' functions this way
namespace SampleRate {
void set(double rate);
double get();
}  // namespace SampleRate
