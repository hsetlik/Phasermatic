#include "Phasermatic/PluginEditor.h"
#include "Phasermatic/Common.h"
#include "Phasermatic/GUI/SpectrumGraph.h"
#include "Phasermatic/Identifiers.h"
#include "Phasermatic/PluginProcessor.h"
#include "Phasermatic/ParameterLayout.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);
  depthSlider.setSliderStyle(juce::Slider::Rotary);
  depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 20);
  speedSlider.setSliderStyle(juce::Slider::Rotary);
  speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 20);
  typeBox.addItemList(getEffectTypeNames(), 1);
  typeBox.setSelectedItemIndex(1);
  addAndMakeVisible(depthSlider);
  addAndMakeVisible(speedSlider);
  addAndMakeVisible(typeBox);
  addAndMakeVisible(graph);
  //  initialize the APVTS attachments
  depthAttach.reset(
      new apvts::SliderAttachment(p.tree, ID::depth.toString(), depthSlider));
  speedAttach.reset(new apvts::SliderAttachment(p.tree, ID::lfoSpeed.toString(),
                                                speedSlider));
  typeAttach.reset(new apvts::ComboBoxAttachment(
      p.tree, ID::phasingType.toString(), typeBox));
  setSize(600, 400);

  // start the timer
  startTimerHz(GRAPH_REFRESH_HZ);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {}

void AudioPluginAudioProcessorEditor::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float typeHeight = 45.0f;
  auto typeBounds = fBounds.removeFromTop(typeHeight);
  typeBox.setBounds(typeBounds.toNearestInt());
  // auto gBounds = fBounds.removeFromTop(0.75f * fBounds.getHeight());
  auto row1 = fBounds.removeFromTop(90.0f);
  graph.setBounds(fBounds.toNearestInt());
  auto lBounds = row1.removeFromLeft(row1.getHeight());
  speedSlider.setBounds(lBounds.toNearestInt());
  lBounds = row1.removeFromLeft(row1.getHeight());
  depthSlider.setBounds(lBounds.toNearestInt());
}
}  // namespace audio_plugin
