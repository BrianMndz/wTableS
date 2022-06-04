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

private:
    float angleDelta = 0.0f;
    float currentAngle = 0.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineOscillator)
};
