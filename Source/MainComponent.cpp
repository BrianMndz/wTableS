#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    cpuUsageLabel.setText ("CPU Usage", juce::dontSendNotification);
    cpuUsageText.setJustificationType(juce::Justification::right);
    addAndMakeVisible(cpuUsageLabel);
    addAndMakeVisible(cpuUsageText);
    
    //Create the wavetable
    singleSine.createWaveTable();
    
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    startTimer(50);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
   /**We have to initialize the oscillators and set their frequencies to play based on the sample rate. */
    auto numberOfOsc = 100;         //Large number of oscillators to evaluate the CPU load of such a number
    
    for(auto i = 0; i < numberOfOsc; i++)
    {
        /** For each oscillator we instantiate a new SineOscillator object. We call the custom constructor */
        //auto * singleOsc = new SineOscillator();
        auto * singleOsc = new WaveTableOscillator(singleSine.sineTable);
        
        /**Select a random midi note using the Random class and shifting the lowest possible note by 4 octaves */
        //auto midiNote = (int)(juce::Random::getSystemRandom().nextDouble() * 36.0 + 60.0);
        auto midiNote = singleSine.chordGenerator();
        /**In order to calculate the freq of that midi note, we use the formula from the A440 and knowing it is 69 note: 440 * 2 ^ (d / 12) */
        auto freq = 440.0 * pow(2.0, (midiNote - 69.0) / 12.0);
        
        std::cout << "Midi: " << midiNote << " freq: " << freq << '\n';
        /**We set the frequency of the oscillator by passing the freq and sample rate as arguments and add the osc to the array of oscillators */
        singleOsc->setFrequency((float)freq, (float)sampleRate);
        oscillators.add(singleOsc);
    }
    
    /**We define the output level by dividing a quien gain level by the number of osc to prevent clip in the sum of signals */
    mainLevel = 0.25f / (float)numberOfOsc;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* leftBuffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
    bufferToFill.clearActiveBufferRegion();
    
    for(auto oscillatorIndex = 0; oscillatorIndex < oscillators.size(); ++oscillatorIndex)
    {
        /**We retrieve  a pointer  to the oscillator instance */
        auto* localOsc = oscillators.getUnchecked(oscillatorIndex);
        
        for(auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            /**For each sample in the audio sample buffer we get the sine wave sample and trim the gain */
            auto levelSample = localOsc->getNextSample() * mainLevel;
            
            leftBuffer[sample] += levelSample;
            rightBuffer[sample] += levelSample;
        }
    }
}

void MainComponent::releaseResources()
{

}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    cpuUsageLabel.setBounds (10, 10, getWidth() - 20, 20);
    cpuUsageText .setBounds (10, 10, getWidth() - 20, 20);
}
