/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TensorFlowVSTAudioProcessorEditor::TensorFlowVSTAudioProcessorEditor (TensorFlowVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    fileComp.reset(new juce::FilenameComponent("fileComp",
        {},                       // current file
        false,                    // can edit file name,
        false,                    // is directory,
        false,                    // is for saving,
        {"*.json"},               // browser wildcard suffix,
        {},                       // enforced suffix,
        "Select model to open"));  // text when nothing selected
    addAndMakeVisible(fileComp.get());
    fileComp->addListener(this);
    fileComp->setDefaultBrowseTarget(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory));

    link.reset(new juce::HyperlinkButton(juce::String("Check the project on github !"), juce::URL("https://github.com/Cyril-Meyer/TensorFlowVST")));
    addAndMakeVisible(link.get());

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 150);
}

TensorFlowVSTAudioProcessorEditor::~TensorFlowVSTAudioProcessorEditor()
{
}

//==============================================================================
void TensorFlowVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("TensorFlowVST", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
    g.drawFittedText(audioProcessor.modelFileName, 0, 80, getWidth(), 30, juce::Justification::centred, 1);
}

void TensorFlowVSTAudioProcessorEditor::resized()
{
    fileComp->setBounds(10, 40, getWidth() - 20, 20);
    link->setBounds(10, 120, getWidth() - 20, 20);
}


void TensorFlowVSTAudioProcessorEditor::filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == fileComp.get())
    {
        audioProcessor.modelFullPathName = fileComp->getCurrentFile().getFullPathName();
        audioProcessor.loadModel();
        repaint();
    }
}