#pragma once

namespace distorter {

    struct Parameters {
            explicit Parameters(juce::AudioProcessor&);

            juce::AudioParameterFloat& gain;
            juce::AudioParameterFloat& distortion;
            juce::AudioParameterInt& mix;
        
            JUCE_DECLARE_NON_COPYABLE(Parameters)
            JUCE_DECLARE_NON_MOVEABLE(Parameters)
    };

} // namespace distorter