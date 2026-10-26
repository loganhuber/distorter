#pragma once

namespace distorter {
    class Distorter {
        public: 
          void setGain(float gain) {
                outputGain.setTargetValue(juce::Decibels::decibelsToGain(gain));
        }

        void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
            outputGain.reset(sampleRate, 0.1f);
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

                for (const auto channelIndex : std::views::iota(0, buffer.getNumChannels())) {

                    const auto inputSample = buffer.getSample(channelIndex, frameIndex);
                    const auto outputSample = inputSample * nextOutputGain;

                    buffer.setSample(channelIndex, frameIndex, outputSample);
                }

            }
        }

        private:
        juce::SmoothedValue<float> outputGain = 1.f;

    }; 


} //namespace distorter