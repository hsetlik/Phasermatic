#pragma once

#include "PluginProcessor.h"
#include "Common.h"
#include "GUI/SpectrumGraph.h"

namespace audio_plugin {

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        juce::Timer {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

  void timerCallback() override {
    graph.refresh(processorRef.getGraphPointer());
  }

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor& processorRef;
  // main controls
  juce::Slider depthSlider;
  juce::Slider speedSlider;
  juce::ComboBox typeBox;
  slider_attach_ptr depthAttach;
  slider_attach_ptr speedAttach;
  combo_attach_ptr typeAttach;

public:
  MultibandGraph graph;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
