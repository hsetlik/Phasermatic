#include "Phasermatic/ParameterLayout.h"
#include "Phasermatic/Common.h"
#include "Phasermatic/Identifiers.h"
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

apvts::ParameterLayout getLayout() {
  apvts::ParameterLayout layout;
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::lfoSpeed.toString(), "LFO Speed", speedRange(), 1.5f));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::depth.toString(), "Mod. Depth", depthRange(), 0.5f));
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      ID::phasingType.toString(), "Effect Type", getEffectTypeNames(), 0));
  return layout;
}
}  // namespace Param
