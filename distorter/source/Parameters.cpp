#pragma once

namespace distorter {

    namespace {
        auto& addParameterToProcessor(juce::AudioProcessor& processor, auto parameter){
        auto& parameterReference = *parameter;
        processor.addParameter(parameter.release());
        return parameterReference;
        }


        juce::AudioParameterFloat& createGainParameter(juce::AudioProcessor& processor) {
            constexpr int versionHint = 1;

            auto parameter = std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID{"Gain", versionHint},
                "Gain", 
                juce::NormalisableRange{0.0f, 1.f, 0.1f},
                1.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }
    }

    Parameters::Parameters(juce::AudioProcessor& processor) :
    gain{createGainParameter(processor)}
    {
        juce::ignoreUnused(processor);
    }

} // namespace distorter