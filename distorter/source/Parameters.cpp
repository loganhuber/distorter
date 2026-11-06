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
                juce::NormalisableRange{0.0f, 24.f, 0.1f},
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
                juce::NormalisableRange{0.0f, 26.f, 0.1f},
                1.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }

        juce::AudioParameterInt& createMixParameter(juce::AudioProcessor& processor) {
            constexpr int versionHint = 1;

            auto parameter = std::make_unique<juce::AudioParameterInt>(
                juce::ParameterID("Mix", versionHint),
                "Dry/Wet",
                0,
                100,
                100,
                juce::AudioParameterIntAttributes{}.withLabel("%")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }

        juce::AudioParameterFloat& createHighPassFreqParameter(juce::AudioProcessor& processor) {
            constexpr int versionHint = 1;

            auto parameter = std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID("Highpass", versionHint),
                "High Pass",
                juce::NormalisableRange{20.f, 500.f, 1.f},
                20.f,
                juce::AudioParameterFloatAttributes{}.withLabel("Hz")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }

        juce::AudioParameterChoice& createSaturationTypeParameter(juce::AudioProcessor& processor) {
            constexpr int versionHint = 1;

            auto parameter = std::make_unique<juce::AudioParameterChoice>(
                juce::ParameterID("Saturation", versionHint),
                "Saturation",
                juce::StringArray{"Atan", "Tanh", "Heavy"},
                0
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }

        juce::AudioParameterFloat& createTrebleControlParameter(juce::AudioProcessor& processor) {
            constexpr int versionHint = 1;

            auto parameter = std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID("High", versionHint),
                "High",
                juce::NormalisableRange{-12.f, 12.f, .1f},
                0.f, 
                juce::AudioParameterFloatAttributes{}.withLabel("dB")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }

        juce::AudioParameterFloat& createMidControlParameter(juce::AudioProcessor& processor) {
            constexpr int versionHint = 1;
            
            auto parameter = std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID("Mid", versionHint),
                "Mid",
                juce::NormalisableRange{-12.f, 12.f, .1f},
                0.f,
                juce::AudioParameterFloatAttributes{}.withLabel("dB")
            );

            return addParameterToProcessor(processor, std::move(parameter));
        }


    }



    Parameters::Parameters(juce::AudioProcessor& processor) :
    gain{createGainParameter(processor)},
    distortion{createDistortionParameter(processor)}, 
    mix{createMixParameter(processor)},
    highPassFreq{createHighPassFreqParameter(processor)},
    saturationType{createSaturationTypeParameter(processor)},
    highControl{createTrebleControlParameter(processor)},
    midControl{createMidControlParameter(processor)}
    {
        juce::ignoreUnused(processor);
    }

} // namespace distorter