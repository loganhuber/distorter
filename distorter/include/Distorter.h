#pragma once

namespace distorter {
    class Distorter {
        public: 
          void setGain(float gain) {
                outputGain.setTargetValue(juce::Decibels::decibelsToGain(gain));
        }

        void setDistortion(float distortion) {
            outputDistortion.setTargetValue(juce::Decibels::decibelsToGain(distortion));
        }

        void setMix(float mix) {
            outputMix.setTargetValue(juce::jlimit(0.f, 1.f, mix));
        }

        void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
            outputGain.reset(sampleRate, 0.1f);
            outputDistortion.reset(sampleRate, 0.1f);
            outputMix.reset(sampleRate, 1);

            juce::ignoreUnused(expectedMaxFramesPerBlock);
            // const juce::dsp::ProcessSpec processSpec {
            //     .sampleRate = sampleRate,
            //     .maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
            //     .numChannels = 1u,
            // };
        }

        void process(juce::AudioBuffer<float>& buffer) noexcept {
            for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {

                const auto nextOutputGain = outputGain.getNextValue();
                const auto nextOutputDistortion = outputDistortion.getNextValue();
                const auto mix = outputMix.getNextValue();

                for (const auto channelIndex : std::views::iota(0, buffer.getNumChannels())) {

                    const auto inputSample = buffer.getSample(channelIndex, frameIndex);
                    const auto shape = std::atan(inputSample * nextOutputDistortion);
                    const auto drivenSample = shape * nextOutputGain;
                    const auto wet = juce::jlimit(-1.f, 1.f, drivenSample);

                    const auto outputSample = inputSample + mix * (wet - inputSample);

                    buffer.setSample(channelIndex, frameIndex, outputSample);
                }

            }
        }

        private:
        juce::SmoothedValue<float> outputGain = 1.f;
        juce::SmoothedValue<float> outputDistortion = 1.f;
        juce::SmoothedValue<float> outputMix = 1;

    }; 


} //namespace distorter