#pragma once

namespace distorter {

    struct Parameters {
            explicit Parameters(juce::AudioProcessor&);

            juce::AudioParameterFloat& gain;
            juce::AudioParameterFloat& distortion;
            juce::AudioParameterInt& mix;
            juce::AudioParameterFloat& highPassFreq;
            juce::AudioParameterChoice& saturationType;

            juce::AudioParameterFloat& highControl;
            juce::AudioParameterFloat& midControl;

        
            JUCE_DECLARE_NON_COPYABLE(Parameters)
            JUCE_DECLARE_NON_MOVEABLE(Parameters)
    };

} // namespace distorter