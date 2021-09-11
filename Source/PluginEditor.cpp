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
        "Select file to open"));  // text when nothing selected
    addAndMakeVisible(fileComp.get());
    fileComp->addListener(this);
    fileComp->setDefaultBrowseTarget(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 100);
}

TensorFlowVSTAudioProcessorEditor::~TensorFlowVSTAudioProcessorEditor()
{
}

//==============================================================================
void TensorFlowVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("TensorFlowVST", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void TensorFlowVSTAudioProcessorEditor::resized()
{
    fileComp->setBounds(10, 40, getWidth() - 20, 20);
}


void TensorFlowVSTAudioProcessorEditor::filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == fileComp.get())
    {
        juce::File file = fileComp->getCurrentFile();
        if (!file.existsAsFile())
            return;

        std::optional<fdeep::model> model = fdeep::load_model(file.getFullPathName().toStdString());
        audioProcessor.model = model;
    }
}