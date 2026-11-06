#pragma once

namespace distorter {
    class Distorter {
        public: 

        enum class SaturationType {
            atan,
            tanh,
            heavy
        };

        void setMidGainDb(float gainDb) {
            midGainDb.setTargetValue(gainDb);
        }

        void setHighGainDb(float gainDb) {
            highGainDb.setTargetValue(gainDb);
        }

        void setSaturationType(SaturationType saturationType) {
            flavor = saturationType;
        }

        void setGain(float gain) {
                outputGain.setTargetValue(juce::Decibels::decibelsToGain(gain));
        }

        void setDistortion(float driveDb) {
            outputDistortion.setTargetValue(juce::Decibels::decibelsToGain(driveDb));

            const auto makeupDB = -0.75f * driveDb;
            outputMakeupGain.setTargetValue(juce::Decibels::decibelsToGain(makeupDB));
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
            highPassFreq.reset(sampleRate, .1f);
            midGainDb.reset(sampleRate, .1f);
            highGainDb.reset(sampleRate, .1f);
            sampleRateHz = sampleRate;
            outputMakeupGain.reset(sampleRate, 0.1f);

            const juce::dsp::ProcessSpec processSpec {
                .sampleRate = sampleRate,
                .maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
                .numChannels = static_cast<juce::uint32>(numChannels),
            };

            highPassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            highPassFilter.setCutoffFrequency(highPassFreq.getCurrentValue());
            highPassFilter.prepare(processSpec);

            midGainDb.setCurrentAndTargetValue(0.0f);
            highGainDb.setCurrentAndTargetValue(0.0f);

            const float neutralGain = 1.0f;

            auto highCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                sampleRate,
                highCenterFreqHz,
                highQ,
                neutralGain
            );

            for (auto& highFilter : highFilters) {
                highFilter.coefficients = highCoefficients;
                highFilter.prepare(processSpec);
            }

            auto midCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                sampleRate,
                midCenterFreqHz,
                midQ,
                neutralGain
            );

            for (auto& midFilter : midFilters) {
                midFilter.coefficients = midCoefficients;
                midFilter.prepare(processSpec);
            }

            
        }

        void process(juce::AudioBuffer<float>& buffer) noexcept {

            midGainDb.skip(buffer.getNumSamples());
            const float midGainFactor = juce::Decibels::decibelsToGain(midGainDb.getCurrentValue());

            *midFilters[0].coefficients = juce::dsp::IIR::ArrayCoefficients<float>::makePeakFilter(
                sampleRateHz, midCenterFreqHz, midQ, midGainFactor
            );

            highGainDb.skip(buffer.getNumSamples());
            const float highGainFactor = juce::Decibels::decibelsToGain(highGainDb.getCurrentValue());

            *highFilters[0].coefficients = juce::dsp::IIR::ArrayCoefficients<float>::makeHighShelf(
                sampleRateHz, highCenterFreqHz, highQ, highGainFactor
            );

            for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {

                const auto nextOutputGain = outputGain.getNextValue();
                const auto nextOutputDistortion = outputDistortion.getNextValue();
                const auto mix = outputMix.getNextValue();
                const auto nextMakeupGain = outputMakeupGain.getNextValue();
                const auto nextHighPassFreq = highPassFreq.getNextValue();
                highPassFilter.setCutoffFrequency(nextHighPassFreq);

                for (const auto channelIndex : std::views::iota(0, buffer.getNumChannels())) {
                    
                    const auto inputSample = buffer.getSample(channelIndex, frameIndex);
                    const auto highPassed = highPassFilter.processSample(channelIndex, inputSample);
                    const auto midProccessed = midFilters[static_cast<size_t>(channelIndex)].processSample(highPassed);
                    const auto filtered = highFilters[static_cast<size_t>(channelIndex)].processSample(midProccessed);

                    auto shape = 0.0f;

                    switch (flavor) {
                        case SaturationType::atan:
                            shape = (nextMakeupGain * .6f) * juce::jlimit(-1.f, 1.f, std::atan(nextOutputDistortion * nextOutputDistortion * filtered));
                            break;
                        case SaturationType::tanh:
                            shape = (nextMakeupGain * .6f) * juce::jlimit(-1.f, 1.f, std::tanh(nextOutputDistortion * nextOutputDistortion * filtered));
                            break;
                        case SaturationType::heavy:
                            shape = nextMakeupGain * juce::jlimit(-1.f, 1.f, std::atan(nextOutputDistortion * nextOutputDistortion * nextOutputDistortion * filtered));
                    }

                    const auto drivenSample = shape * nextOutputGain;
                    const auto outputSample = inputSample + mix * (drivenSample - inputSample);

                    buffer.setSample(channelIndex, frameIndex, outputSample);
                }

            }
        }

        private:
        juce::SmoothedValue<float> outputGain = 1.f;
        juce::SmoothedValue<float> outputDistortion = 1.f;
        juce::SmoothedValue<float> outputMix = 1;
        juce::SmoothedValue<float> highPassFreq = 20;

        juce::SmoothedValue<float> outputMakeupGain = 1.0f;
        juce::dsp::StateVariableTPTFilter<float> highPassFilter;

        SaturationType flavor = SaturationType::atan;

        double sampleRateHz = 0.0;

        juce::SmoothedValue<float> midGainDb = 1.0f;
        std::array<juce::dsp::IIR::Filter<float>, 2> midFilters;
        const float midCenterFreqHz = 1000.0f;
        const float midQ = 1.f;

        juce::SmoothedValue<float> highGainDb = 1.0f;
        std::array<juce::dsp::IIR::Filter<float>, 2> highFilters;
        const float highCenterFreqHz = 3000.0f;
        const float highQ = 5.f;


    }; 


} //namespace distorter