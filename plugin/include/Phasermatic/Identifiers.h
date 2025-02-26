#pragma once

#include <juce_core/juce_core.h>
typedef juce::NormalisableRange<float> frange_t;

#define DECLARE_ID(name) const juce::Identifier name(#name);
// wrapper so we don't need to remember every parameter's string literal
namespace ID {
DECLARE_ID(lfoSpeed)
DECLARE_ID(phasingType)
DECLARE_ID(depth)

}  // namespace ID

// #undef DECLARE_ID
