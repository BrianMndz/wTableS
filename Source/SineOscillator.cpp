/*
  ==============================================================================

    SineOscillator.cpp
    Created: 4 Jun 2022 1:01:59am
    Author:  Brian Mendoza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SineOscillator.h"

//==============================================================================
SineOscillator::SineOscillator()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SineOscillator::~SineOscillator()
{
}

void SineOscillator::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("SineOscillator", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void SineOscillator::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

int SineOscillator::chordGenerator()
{
    /**Midi notes of a Cmaj7 chord in 4 octaves */
    int chordNotes[16] = {60, 64, 67, 71,
                          72, 76, 79, 83,
                          84, 88, 91, 95,
                          96, 100,103,107};
    
    auto midiNote = (int)(juce::Random::getSystemRandom().nextDouble() * 16.0);
    
    return chordNotes[midiNote];
}
