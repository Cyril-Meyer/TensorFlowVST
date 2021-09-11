/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TensorFlowVSTAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                           public juce::FilenameComponentListener
{
public:
    TensorFlowVSTAudioProcessorEditor (TensorFlowVSTAudioProcessor&);
    ~TensorFlowVSTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TensorFlowVSTAudioProcessor& audioProcessor;

    std::unique_ptr<juce::FilenameComponent> fileComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TensorFlowVSTAudioProcessorEditor)
};
