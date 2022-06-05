/*
  ==============================================================================

    SineOscillator.h
    Created: 4 Jun 2022 1:01:59am
    Author:  Brian Mendoza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SineOscillator  : public juce::Component
{
public:
    SineOscillator();
    
    ~SineOscillator() override;
    
    /** Allows us to calculate the angle delta by first dividing the frequency by the sample rate.
     Then multiplies by 2 pi to get the length of a cycle in radians */
    void setFrequency(float frequency, float sampleRate)
    {
        auto cyclesPerSample = frequency / sampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<float>::twoPi;
    }
    
    /**This function will be called by getNextAudioBlock() on every sample in the buffer to retrieve the sample value from the oscillator.
     We calculate the sample value using the std::sin() function by passing the current angle as an argument and updating
     the current angle by calling the helper function below. */
    forcedinline float getNextSample() noexcept
    {
        auto currentSample = std::sin(currentAngle);
        updateAngle();
        return currentSample;
    }
    
    /**The angle is updated by incrementing the current angle with the angle delta calculated previously when setting the frequency
     and by wrapping the angle when exceeds 2pi */
    forcedinline void updateAngle() noexcept
    {
        currentAngle += angleDelta;
        if(currentAngle >= juce::MathConstants<float>::twoPi)
            currentAngle -= juce::MathConstants<float>::twoPi;
    }

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int chordGenerator();
    
    /**Function that will be called in the mainComponent before the audio processing */
    void createWaveTable();
    
    /**Variable to hold the wavetable values of single sine wave cycle */
    const unsigned int tableSize = 1 << 7;      //Mamalord: bitshift to represent 128...
    juce::AudioSampleBuffer sineTable;

private:
    float angleDelta = 0.0f;
    float currentAngle = 0.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineOscillator)
};

class WaveTableOscillator
{
public:
    WaveTableOscillator(const juce::AudioSampleBuffer& wavetableToUse) : wavetable(wavetableToUse)
    {
        jassert(wavetable.getNumChannels() == 1);
    }
    
    /**Instead of keeping track of the current angle and the angle delta, define two member variables that store the current wavetable index*/
    void setFrequency(float frequency, float sampleRate)
    {
        auto tableSizeOverSampleRate = (float)wavetable.getNumSamples() / sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }
    
    forcedinline float getNextSample() noexcept
    {
        auto tableSize = (unsigned int) wavetable.getNumSamples();
        /**TEmporary store the to indices of wt surrounding the sample value to retrieve. If higher index goes beyond the size, wrap the value to the start. */
        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int) 0 : index0 + 1;
        /**CAlculate the interpolation value as fraction between two indices */
        auto fracc = currentIndex - (float)index0;
        /**Get a pointer to the audio sample and read the values at the two indices and store temporary*/
        auto* table = wavetable.getReadPointer(0);
        auto value0 = table[index0];
        auto value1 = table[index1];
        /**The interpolated sample value can then be retrieved by using the standard interpolation formula*/
        auto currentSample = value0 + fracc * (value1 - value0);
        
        /**Increment the angle delta of the table and wrap the value around if the value exceed*/
        if( (currentIndex += tableDelta) > (float)tableSize )
            currentIndex -= (float)tableSize;
        
        return currentSample;
    }
    
private:
    float tableDelta = 0.0f;
    float currentIndex = 0.0f;
    const juce::AudioSampleBuffer& wavetable;

};
