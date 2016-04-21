/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "Vibrato.h"
#include "ErrorDef.h"
#include "Ppm.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    void processBlockBypassed (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void setParameter (int parameter_index, float new_value) override;
    float getParameter(int parameter_index) override;
    
    
    //==============================================================================
    bool getBypassedState();
    void setBypassedState(bool state);
    
    float getPeakMeterValue();

private:
    CVibrato* _vibrato;
    Ppm* _ppm;
    Error_t _error_check;
    AudioParameterFloat* _mod_frequency;
    AudioParameterFloat* _mod_amplitude;
    bool _is_bypassed;
    bool _param_updated;
    float _temp_mod_freq;
    float _temp_mod_amp;
    float* _ppm_value;
	float* _max_ppm_value;
    
    void setParameters();
    void setParametersBypassed();
    //bool getParamUpdateState();
    //void setParamUpdateState(bool state);
    
 
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
