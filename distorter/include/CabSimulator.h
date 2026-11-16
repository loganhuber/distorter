#pragma once 

namespace distorter {

    class CabSimulator {
        public:
        CabSimulator() 
        {
            auto& convolution = processorChain.get<convolutionIndex>();

            convolution.loadImpulseResponse(
                DistorterBinaryData::classic50impulse_wav,
                static_cast<size_t>(DistorterBinaryData::classic50impulse_wavSize),
                juce::dsp::Convolution::Stereo::yes,
                juce::dsp::Convolution::Trim::no,
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