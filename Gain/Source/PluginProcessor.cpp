/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define MIN_MOD_AMP 0.0
#define MAX_MOD_AMP 0.1
#define DEFAULT_MOD_AMP 0.01
#define MIN_MOD_FREQ 0.0
#define MAX_MOD_FREQ 20.0
#define DEFAULT_MOD_FREQ 2.0

using namespace std;

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
{
    _vibrato = 0;
    _ppm = 0;
    _ppm_value = 0;
	_max_ppm_value = 0;
    _is_bypassed = false;
    _param_updated = false;
    _mod_frequency = new AudioParameterFloat("mod freq","Modulation Frequency", MIN_MOD_FREQ, MAX_MOD_FREQ, DEFAULT_MOD_FREQ);
    addParameter(_mod_frequency);
    _mod_amplitude = new AudioParameterFloat("mod amp","Modulation Amplitude", MIN_MOD_AMP, MAX_MOD_AMP, DEFAULT_MOD_AMP);
    addParameter(_mod_amplitude);
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
    CVibrato::destroyInstance(_vibrato);
    Ppm::destroyInstance(_ppm);
    delete [] _ppm_value;
	delete[] _max_ppm_value;
}

//==============================================================================
const String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
	return false;
}

bool NewProjectAudioProcessor::producesMidi() const
{
	return false;
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const String NewProjectAudioProcessor::getProgramName (int index)
{
    return String();
}

void NewProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    int num_channels = max(totalNumOutputChannels, totalNumInputChannels);
    
    if(_vibrato != nullptr) {
        CVibrato::destroyInstance(_vibrato);
    }
    _error_check = CVibrato::createInstance(_vibrato);
    if (_error_check == kUnknownError)
    {
        cerr << "Runtime error. Memory issues." << endl;
    }
    
    
    if(_ppm != nullptr) {
        Ppm::destroyInstance(_ppm);
    }
    _error_check = Ppm::createInstance(_ppm);
    if (_error_check == kUnknownError)
    {
        cerr << "Runtime error. Memory issues." << endl;
    }
    
    _error_check = _vibrato->initInstance(MAX_MOD_AMP ,sampleRate, num_channels);
    if (_error_check == kFunctionInvalidArgsError)
    {
        cerr << "Invalid parameters: One or more parameters is out of bounds. Please check your parameters." << endl;
    }
    
    _error_check = _ppm->initInstance(sampleRate, num_channels);
    if (_error_check == kFunctionInvalidArgsError)
    {
        cerr << "Invalid parameters: One or more parameters is out of bounds. Please check your parameters." << endl;
    }
    
    
    _ppm_value = new float [num_channels];
	_max_ppm_value = new float[num_channels];
    for (int i = 0; i < num_channels; i++) {
        _ppm_value[i] = 0;
		_max_ppm_value[i] = 0;
    }

    //initialize with default parameters
    setParameter(0, getParameterDefaultValue(0));
    setParameter(1, getParameterDefaultValue(1));
    setParameters();
    
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void NewProjectAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if (_is_bypassed)
    {
        processBlockBypassed(buffer, midiMessages);
        return;
    }
    else
    {
        if (_param_updated)
        {
            setParameters();
        }
        float** write_pointer = buffer.getArrayOfWritePointers();
        _vibrato->process(write_pointer, write_pointer, buffer.getNumSamples());
        //const float** read_pointer = buffer.getArrayOfReadPointers();
        _ppm->process(write_pointer, buffer.getNumSamples(), _ppm_value);
		if (_ppm_value[0] > _max_ppm_value[0])
			_max_ppm_value[0] = _ppm_value[0];
		if (_ppm_value[1] > _max_ppm_value[1])
			_max_ppm_value[1] = _ppm_value[1];
    }
}

void NewProjectAudioProcessor::processBlockBypassed (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    setParametersBypassed();
    float** write_pointer = buffer.getArrayOfWritePointers();
    _vibrato->process(write_pointer, write_pointer, buffer.getNumSamples());
    //const float** read_pointer = buffer.getArrayOfReadPointers();
    _ppm->process(write_pointer, buffer.getNumSamples(), _ppm_value);
	if (_ppm_value[0] > _max_ppm_value[0])
		_max_ppm_value[0] = _ppm_value[0];
	if (_ppm_value[1] > _max_ppm_value[1])
		_max_ppm_value[1] = _ppm_value[1];
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void NewProjectAudioProcessor::setParameter(int parameter_index, float new_value)
{
    if (parameter_index == 0)
    {
        _temp_mod_freq = new_value * (MAX_MOD_FREQ - MIN_MOD_FREQ) + MIN_MOD_FREQ;
        _param_updated = true;
    }
    else if (parameter_index == 1)
    {
        _temp_mod_amp = new_value * (MAX_MOD_AMP - MIN_MOD_AMP) + MIN_MOD_AMP;
        _param_updated = true;
    }
    else
    {
        jassert("Invalid parameter index");
    }
}

float NewProjectAudioProcessor::getParameter(int parameter_index)
{
    if (parameter_index == 0)
    {
        float value = _vibrato->getParam(CVibrato::VibratoParam_t::kParamModFreqInHz);
        value = (value - MIN_MOD_FREQ) / (MAX_MOD_FREQ - MIN_MOD_FREQ);
        return value;
    }
    else if (parameter_index == 1)
    {
        float value = _vibrato->getParam(CVibrato::VibratoParam_t::kParamModWidthInS);
        value = (value - MIN_MOD_AMP) / (MAX_MOD_AMP - MIN_MOD_AMP);
        return value;
    }
    else
    {
        jassert("Invalid Parameter Index");
    }
}

bool NewProjectAudioProcessor::getBypassedState()
{
    return _is_bypassed;
}

void NewProjectAudioProcessor::setBypassedState(bool state)
{
    _is_bypassed = state;
}

float NewProjectAudioProcessor::getPeakMeterValue() {
	float return_val = _max_ppm_value[0];
	_max_ppm_value[0] = 0;
	_max_ppm_value[1] = 0;
    return return_val;
}

/*
bool NewProjectAudioProcessor::getParamUpdateState()
{
    return _param_updated;
}

void NewProjectAudioProcessor::setParamUpdateState(bool state)
{
    _param_updated = state;
}
*/
void NewProjectAudioProcessor::setParameters()
{
    if (_param_updated)
    {
        _vibrato->setParam(CVibrato::VibratoParam_t::kParamModFreqInHz, _temp_mod_freq);
        _vibrato->setParam(CVibrato::VibratoParam_t::kParamModWidthInS, _temp_mod_amp);
        _param_updated = false;
    }
    else
    {
        return;
    }
}

void NewProjectAudioProcessor::setParametersBypassed()
{
    _vibrato->setParam(CVibrato::VibratoParam_t::kParamModFreqInHz, 0);
    _vibrato->setParam(CVibrato::VibratoParam_t::kParamModWidthInS, 0);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
