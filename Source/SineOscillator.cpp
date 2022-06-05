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

}

SineOscillator::~SineOscillator()
{
}

void SineOscillator::paint (juce::Graphics& g)
{
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
    int chordNotes[20] = {48, 52, 55, 59,
                          60, 64, 67, 71,
                          72, 76, 79, 83,
                          84, 88, 91, 95,
                          96, 100,103,107};
    
    auto midiNote = (int)(juce::Random::getSystemRandom().nextDouble() * 20.0);
    
    return chordNotes[midiNote];
}

void SineOscillator::createWaveTable()
{
    sineTable.setSize(1, (int) tableSize);
    /**Initialize the AudioSampleBuffer by calling setSize and we only need 1 chonles and retrieve the write pointer for that single channel buffer*/
    auto* samples = sineTable.getWritePointer(0);
    /**calculate the angle delta but now using  the table size between the full 2pi cycle by 127*/
    auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
    auto currentAngle = 0.0;
    
    for(unsigned int i = 0; i < tableSize; ++i)
    {
        auto sample = std::sin(currentAngle);
        samples[i] = (float)sample;
        currentAngle += angleDelta;
    }
}
