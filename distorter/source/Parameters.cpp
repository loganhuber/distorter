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
                juce::NormalisableRange{0.0f, 10.f, 0.1f},
                5.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }

        juce::AudioParameterFloat& createDistortionParameter(juce::AudioProcessor& processor) {
            constexpr int versionHint = 1;

            auto parameter = std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID("Distortion", versionHint), 
                "Distortion", 
                juce::NormalisableRange{0.0f, 24.f, 0.1f},
                1.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }
    }

    Parameters::Parameters(juce::AudioProcessor& processor) :
    gain{createGainParameter(processor)},
    distortion{createDistortionParameter(processor)}
    {
        juce::ignoreUnused(processor);
    }

} // namespace distorter