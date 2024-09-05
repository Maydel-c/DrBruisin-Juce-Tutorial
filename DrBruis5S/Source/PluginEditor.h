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
class DrBruis5SAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DrBruis5SAudioProcessorEditor (DrBruis5SAudioProcessor&);
    ~DrBruis5SAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DrBruis5SAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrBruis5SAudioProcessorEditor)
};
