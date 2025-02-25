#include "Phasermatic/ParameterLayout.h"
#include "juce_audio_processors/juce_audio_processors.h"

namespace Param {
// range for the LFO speed in hZ
frange_t speedRange() {
  frange_t range(0.05f, 15.0f);
  range.setSkewForCentre(1.5f);
  return range;
}
frange_t depthRange() {
  frange_t range(0.0f, 1.0f);
  return range;
}

juce::StringArray phasingTypes() {
  juce::StringArray arr;
  arr.add("RandomOffset");
  return arr;
}
apvts::ParameterLayout getLayout() {
  apvts::ParameterLayout layout;
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      "lfoSpeed", "LFO Speed", speedRange(), 1.5f));
  layout.add(std::make_unique<juce::AudioParameterFloat>("depth", "Mod Depth",
                                                         depthRange(), 0.5f));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      "phasingType", "Phasing Type", phasingTypes(), 0));
  return layout;
}
}  // namespace Param
