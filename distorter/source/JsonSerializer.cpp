namespace distorter {

  namespace 
  {
    struct SerializableParameters {

        float gain;
        float distortion;
        int mix;
        float highPassFreq;
        int saturationType; // index, not the string name

      static constexpr auto marshallingVersion = 1;

      template <typename Archive, typename T>
      static void serialise(Archive& archive, T& t) {

        using namespace juce;

        if (archive.getVersion() != 1) {
          return;
        }

        std::string pluginName = JUCE_PLUGIN_NAME;
        archive(named("pluginName", pluginName));

        if (pluginName != JUCE_PLUGIN_NAME) {
          return;
        }

        archive(named("gain", t.gain),
                named("distortion", t.distortion), 
                named("mix", t.mix),
                named("highPassFreq", t.highPassFreq),
                named("saturationType", t.saturationType)
            );

      }
    };

    SerializableParameters from(const Parameters& parameters) {
      return {
        .gain = parameters.gain.get(),
        .distortion = parameters.distortion.get(),
        .mix = parameters.mix.get(),
        .highPassFreq = parameters.highPassFreq.get(),
        .saturationType = parameters.saturationType.getIndex()
      };
    }
  } // unnamed namespace 
  
void JsonSerializer::serialize(const Parameters& parameters,
                               juce::OutputStream& output) {

    const auto parametersToSerialize = from(parameters);
    const auto json = juce::ToVar::convert(parametersToSerialize);

    if (!json.has_value()) {
      return;
    }

    juce::JSON::writeToStream(
                            output, 
                            *json, 
                            juce::JSON::FormatOptions{}
                            .withSpacing(juce::JSON::Spacing::multiLine)
                            .withMaxDecimalPlaces(2)
                          );
}

juce::Result JsonSerializer::deserialize(juce::InputStream& input,
                                         Parameters& parameters) {
  juce::ignoreUnused(input, parameters);

  juce::var parsedResult;
  const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

  if (result.failed()) {
    return result;
  }

  const auto parsedParameters = juce::FromVar::convert<SerializableParameters>(parsedResult);

  if (!parsedParameters.has_value()) {
    return juce::Result::fail("Failed to deserialize json");
  }

  parameters.gain = parsedParameters->gain;
  parameters.distortion = parsedParameters->distortion;
  parameters.mix = parsedParameters->mix;
  parameters.highPassFreq = parsedParameters->highPassFreq;
  parameters.saturationType = parsedParameters->saturationType;

  return juce::Result::ok();
}
}  // namespace distorter