#pragma once 

namespace distorter {

    class CabSimulator {
        public:

        enum class CabType {
            peavey,
            marshall
        };

        CabSimulator() 
        {
            loadImpulseResponseFor(CabType::peavey);
        }

        void loadImpulseResponseFor(CabType requested) {
            auto& convolution = processorChain.get<convolutionIndex>();

            switch (requested) {
                case CabType::peavey:
                    convolution.loadImpulseResponse(
                        DistorterBinaryData::classic50impulse_wav,
                        static_cast<size_t>(DistorterBinaryData::classic50impulse_wavSize),
                        juce::dsp::Convolution::Stereo::no,
                        juce::dsp::Convolution::Trim::yes,
                        0);
                    break;
                case CabType::marshall:              
                    convolution.loadImpulseResponse(
                    DistorterBinaryData::marshallImpulse_wav,
                    static_cast<size_t>(DistorterBinaryData::marshallImpulse_wavSize),
                    juce::dsp::Convolution::Stereo::no,
                    juce::dsp::Convolution::Trim::yes,
                    0);
                    break;
            }
        }
        
        // void requestCabType(CabType type)
        // {
        //     requestedCabType.store(type);
        // }
        
        void reset() noexcept {
            processorChain.reset();
        }

        void prepare(const juce::dsp::ProcessSpec& spec) {
            processorChain.prepare(spec);
        }

        template <typename ProcessContext>
        void process(const ProcessContext& context) noexcept {
            const auto requested = requestedCabType.load(std::memory_order_relaxed);
            if (requested != activeCabType) {
                loadImpulseResponseFor(requested);
                activeCabType = requested;
            }

            processorChain.process(context);
        }

        void setCabType(CabType cabType) noexcept {
            requestedCabType.store(cabType, std::memory_order_relaxed);
        }


    private:
        enum {
            convolutionIndex
        };

        juce::dsp::ProcessorChain<juce::dsp::Convolution> processorChain;
        std::atomic<CabType> requestedCabType{CabType::peavey};
        CabType activeCabType = CabType::peavey;
        
    };

}
 // namespace distorter