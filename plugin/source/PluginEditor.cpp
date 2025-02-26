#include "Phasermatic/PluginEditor.h"
#include "Phasermatic/Common.h"
#include "Phasermatic/Identifiers.h"
#include "Phasermatic/PluginProcessor.h"
#include "Phasermatic/ParameterLayout.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);
  depthSlider.setSliderStyle(juce::Slider::Rotary);
  depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 45, 12);
  speedSlider.setSliderStyle(juce::Slider::Rotary);
  speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 45, 12);
  typeBox.addItemList(getEffectTypeNames(), 1);
  typeBox.setSelectedItemIndex(1);
  addAndMakeVisible(depthSlider);
  addAndMakeVisible(speedSlider);
  addAndMakeVisible(typeBox);
  // initialize the APVTS attachments
  depthAttach.reset(
      new apvts::SliderAttachment(p.tree, ID::depth.toString(), depthSlider));
  speedAttach.reset(new apvts::SliderAttachment(p.tree, ID::lfoSpeed.toString(),
                                                speedSlider));
  typeAttach.reset(new apvts::ComboBoxAttachment(
      p.tree, ID::phasingType.toString(), typeBox));
  setSize(600, 400);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {}

void AudioPluginAudioProcessorEditor::resized() {
  auto fBounds = getLocalBounds().toFloat();
  auto typeBounds = fBounds.removeFromTop(fBounds.getHeight() * 0.2f);
  typeBox.setBounds(typeBounds.toNearestInt());
  auto lBounds = fBounds.removeFromLeft(fBounds.getWidth() / 2.0f);
  speedSlider.setBounds(lBounds.toNearestInt());
  depthSlider.setBounds(fBounds.toNearestInt());
}
}  // namespace audio_plugin
