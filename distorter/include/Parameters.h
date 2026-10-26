#pragma once

namespace distorter {

    struct Parameters {
            explicit Parameters(juce::AudioProcessor&);

            juce::AudioParameterFloat& gain;
        
            JUCE_DECLARE_NON_COPYABLE(Parameters)
            JUCE_DECLARE_NON_MOVEABLE(Parameters)
    };

} // namespace distorter