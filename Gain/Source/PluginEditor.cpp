/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);

    // set slider properties
	_slider_mod_freq.setSliderStyle(Slider::LinearBarVertical);
	_slider_mod_freq.setRange(0, 1);
    
    std::cout << processor.getParameter(0) << std::endl;
    std::cout << processor.getParameter(0) << std::endl;
    
    _slider_mod_freq.setValue(processor.getParameter(0));
    addAndMakeVisible(_slider_mod_freq);
    _slider_mod_freq.addListener(this);
    
    _slider_mod_amp.setSliderStyle(Slider::LinearBarVertical);
    _slider_mod_amp.setRange(0,1);
    _slider_mod_amp.setValue(processor.getParameter(1));
    addAndMakeVisible(_slider_mod_amp);
    _slider_mod_amp.addListener(this);
    
    _toggle_button.setColour (TextButton::buttonColourId, Colours::lightgreen);
    _toggle_button.setButtonText("Bypass");
    addAndMakeVisible(_toggle_button);
    _toggle_button.addListener(this);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawText("Vibrat oOoOoOo", getWidth()/2-100, 5 , 200, 25, Justification::centred);
    g.drawText("Mod. Frequency", getWidth()/2-120, 210, 100, 20, Justification::centred);
    g.drawText("Mod. Amplitude", getWidth()/2+25, 210, 100, 20, Justification::centred);
    //g.drawFittedText ("Vibrat oOoOoOo", getLocalBounds(), Justification::centred, 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    _slider_mod_freq.setBounds(getWidth() / 2 - 85, 50, 40, 150);
    _slider_mod_amp.setBounds(getWidth()/2 + 56, 50, 40, 150);
    _toggle_button.setBounds(getWidth() / 2 - 30, 260, 60, 20);
}

void NewProjectAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &_toggle_button)
    {
        bool cur_state = processor.getBypassedState();
        if (cur_state)
        {
            _toggle_button.setButtonText("Bypass");
            _toggle_button.setColour (TextButton::buttonColourId, Colours::lightgreen);
        }
        else
        {
            _toggle_button.setButtonText("Bypassed");
            _toggle_button.setColour (TextButton::buttonColourId, Colours::lightgrey);
        }
        bool next_state = !cur_state;
        processor.setBypassedState(next_state);
    }
}

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &_slider_mod_freq)
    {
        float mod_freq_value = _slider_mod_freq.getValue();
        processor.setParameter(0, mod_freq_value);
    }
    if (slider == &_slider_mod_amp)
    {
        float mod_amp_value = _slider_mod_amp.getValue();
        processor.setParameter(1, mod_amp_value);
    }
}

