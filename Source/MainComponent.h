#pragma once

#include <JuceHeader.h>
#include "SineOscillator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
                      public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    /**Function from juce::Timer if not override, will get an error */
    void timerCallback() override
    {
        auto cpu = deviceManager.getCpuUsage() * 100;
        cpuUsageText.setText (juce::String (cpu, 6) + " %", juce::dontSendNotification);
    }

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::Label cpuUsageLabel;
    juce::Label cpuUsageText;
    
    float mainLevel = 0.0f;
    /**Array designed for holding objects.
     Holds a list of pointer to objects and will automatically delete the objects when they are removed from the array.
     ow.add(myObj)   after adding objects, they are "owned" by the array. */
    juce::OwnedArray<SineOscillator> oscillators;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
