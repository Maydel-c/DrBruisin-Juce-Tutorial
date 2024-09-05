/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DrBruis5SAudioProcessor::DrBruis5SAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

DrBruis5SAudioProcessor::~DrBruis5SAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout DrBruis5SAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto pGain = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"gain", 1}, "Gain", -24.0, 24.0, 0.0);
    auto pPhase = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"phase", 1}, "Phase", false);
    
    params.push_back(std::move(pGain));
    params.push_back(std::move(pPhase));
    
    return { params.begin(), params.end()};
}

//==============================================================================
const juce::String DrBruis5SAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DrBruis5SAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DrBruis5SAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DrBruis5SAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DrBruis5SAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DrBruis5SAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DrBruis5SAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DrBruis5SAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DrBruis5SAudioProcessor::getProgramName (int index)
{
    return {};
}

void DrBruis5SAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DrBruis5SAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DrBruis5SAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DrBruis5SAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DrBruis5SAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    float dbGain =*treeState.getRawParameterValue("gain");
//    float rawGain = std::pow(10, dbGain / 20);
    float rawGain = juce::Decibels::decibelsToGain(dbGain);
    
    bool isPhase = *treeState.getRawParameterValue("phase");

    juce::dsp::AudioBlock<float> block (buffer);
    
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            if(isPhase)
            {
                channelData[sample] *= rawGain * -1.0;
            } else {
                channelData[sample] *= rawGain;
            }
        }
    }
}

//==============================================================================
bool DrBruis5SAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DrBruis5SAudioProcessor::createEditor()
{
//    return new DrBruis5SAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void DrBruis5SAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DrBruis5SAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DrBruis5SAudioProcessor();
}
