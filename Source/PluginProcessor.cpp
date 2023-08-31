/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViraFilterAudioProcessor::ViraFilterAudioProcessor()
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

ViraFilterAudioProcessor::~ViraFilterAudioProcessor()
{
}

//==============================================================================
const juce::String ViraFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ViraFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ViraFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ViraFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ViraFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ViraFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ViraFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ViraFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ViraFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void ViraFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ViraFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    filterL.initializeFilter(sampleRate);
    filterR.initializeFilter(sampleRate);
}

void ViraFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ViraFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ViraFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();
    int sampleCount = buffer.getNumSamples();
    
    float gain = apvts.getRawParameterValue("gain")->load();
    float frq = apvts.getRawParameterValue("frequency")->load();
    float res = apvts.getRawParameterValue("resonance")->load();
    filterL.updateComponents(frq, res);
    filterR.updateComponents(frq, res);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        switch (channel) {
            case 0:
                for (int i = 0; i < sampleCount; ++i)
                {
                    channelData[i] = filterL.processSample(channelData[i]) * gain;
                }
                break;
            case 1:
                for (int i = 0; i < sampleCount; ++i)
                {
                    channelData[i] = filterR.processSample(channelData[i]) * gain;
                }
                break;
            default:
                break;
        }
    }
}

//==============================================================================
bool ViraFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ViraFilterAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
    // return new ViraFilterAudioProcessorEditor (*this);
}

//==============================================================================
// Parameters for the Plugin
juce::AudioProcessorValueTreeState::ParameterLayout
    ViraFilterAudioProcessor::createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>
               (juce::ParameterID("gain", 1),
                "Gain",
                juce::NormalisableRange<float> (0.1, 5.0, 0.01, 1.0),
                1.0)
               );
    
    layout.add(std::make_unique<juce::AudioParameterFloat>
               (juce::ParameterID("frequency", 1),
                "frq",
                // Butterworth
                juce::NormalisableRange<float> (500.0e-6, 400.0e-3, 1e-6, 0.2),
                16.0e-3)
                // LPF2
//                juce::NormalisableRange<float> (10.0, 20.0e3, 1.0, 0.2),
//                1000)
               );
    
    layout.add(std::make_unique<juce::AudioParameterFloat>
               (juce::ParameterID("resonance", 1),
                "res",
                // Butterworth
                juce::NormalisableRange<float> (50.0, 3000.0, 1.0, 0.2),
                50.0)
                // LPF2
//                juce::NormalisableRange<float> (0.1, 10, 0.1, 0.2),
//                1.0)
               );
    
    return layout;
}

//==============================================================================
void ViraFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ViraFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ViraFilterAudioProcessor();
}
