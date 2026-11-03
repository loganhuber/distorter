#pragma once

namespace distorter {
    class Distorter {
        public: 

        enum class SaturationType {
            atan,
            tanh
        };

        void setSaturationType(SaturationType saturationType) {
            flavor = saturationType;
        }

        void setGain(float gain) {
                outputGain.setTargetValue(juce::Decibels::decibelsToGain(gain));
        }

        void setDistortion(float distortion) {
            outputDistortion.setTargetValue(juce::Decibels::decibelsToGain(distortion));
        }

        void setMix(float mix) {
            outputMix.setTargetValue(juce::jlimit(0.f, 1.f, mix));
        }

        void setHighPass(float highPass) {
            highPassFreq.setTargetValue(highPass);
        }

        void prepare(double sampleRate, int expectedMaxFramesPerBlock, int numChannels) {
            outputGain.reset(sampleRate, 0.1f);
            outputDistortion.reset(sampleRate, 0.1f);
            outputMix.reset(sampleRate, 1);
            highPassFreq.reset(sampleRate, .6f);

            const juce::dsp::ProcessSpec processSpec {
                .sampleRate = sampleRate,
                .maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
                .numChannels = static_cast<juce::uint32>(numChannels),
            };

            highPassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            highPassFilter.setCutoffFrequency(highPassFreq.getCurrentValue());
            highPassFilter.prepare(processSpec);
        }

        void process(juce::AudioBuffer<float>& buffer) noexcept {
            for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {

                const auto nextOutputGain = outputGain.getNextValue();
                const auto nextOutputDistortion = outputDistortion.getNextValue();
                const auto mix = outputMix.getNextValue();

                const auto nextHighPassFreq = highPassFreq.getNextValue();
                highPassFilter.setCutoffFrequency(nextHighPassFreq);
                
                for (const auto channelIndex : std::views::iota(0, buffer.getNumChannels())) {
                    
                    const auto inputSample = buffer.getSample(channelIndex, frameIndex);
                    const auto filtered = highPassFilter.processSample(channelIndex, inputSample);
                    
                    auto shape = 0.0f;

                    switch (flavor) {
                        case SaturationType::atan:
                            shape = std::atan(filtered * nextOutputDistortion);
                            break;
                        case SaturationType::tanh:
                            shape = std::tanh(filtered * nextOutputDistortion);
                            break;
                    }

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
        juce::SmoothedValue<float> highPassFreq = 20;

        juce::dsp::StateVariableTPTFilter<float> highPassFilter;

        SaturationType flavor = SaturationType::atan;


    }; 


} //namespace distorter