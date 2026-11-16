#pragma once 

namespace distorter {

    class CabSimulator {
        public:
        CabSimulator() 
        {
            auto& convolution = processorChain.get<convolutionIndex>();

            convolution.loadImpulseResponse(
                DistorterBinaryData::marshallImpulse_wav,
                static_cast<size_t>(DistorterBinaryData::marshallImpulse_wavSize),
                juce::dsp::Convolution::Stereo::no,
                juce::dsp::Convolution::Trim::yes,
                0);
        }

        
        void reset() noexcept {
            processorChain.reset();
        }

        void prepare(const juce::dsp::ProcessSpec& spec) {
            processorChain.prepare(spec);
        }

        template <typename ProcessContext>
        void process(const ProcessContext& context) noexcept {
            processorChain.process(context);
        }


    private:
        enum {
            convolutionIndex
        };

        juce::dsp::ProcessorChain<juce::dsp::Convolution> processorChain;
    };

}
 // namespace distorter