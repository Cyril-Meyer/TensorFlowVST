/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TensorFlowVSTAudioProcessor::TensorFlowVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

TensorFlowVSTAudioProcessor::~TensorFlowVSTAudioProcessor()
{
}

//==============================================================================
const juce::String TensorFlowVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TensorFlowVSTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TensorFlowVSTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TensorFlowVSTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TensorFlowVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TensorFlowVSTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TensorFlowVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TensorFlowVSTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TensorFlowVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void TensorFlowVSTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TensorFlowVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TensorFlowVSTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TensorFlowVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TensorFlowVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (!model.has_value())
        return;

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        std::vector<float> signal;

        for (int i = 0; i < numSamples; ++i)
        {
            signal.push_back(channelData[i]);
        }

        const fdeep::tensor t(fdeep::tensor_shape(numSamples, 1), signal);

        const auto result = model.value().predict({ t });

        for (int i = 0; i < numSamples; ++i)
        {
            channelData[i] = result.front().get(fdeep::tensor_pos(i, 0));
        }
    }
}

//==============================================================================
bool TensorFlowVSTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TensorFlowVSTAudioProcessor::createEditor()
{
    return new TensorFlowVSTAudioProcessorEditor (*this);
}

//==============================================================================
void TensorFlowVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void TensorFlowVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TensorFlowVSTAudioProcessor();
}
