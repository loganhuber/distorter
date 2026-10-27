/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            distorter
   vendor:        WolfSound
   version:       0.1.0
   name:          Distorter
   description:   Plugin core
   dependencies:  juce_audio_utils juce_dsp

   website:       https://thewolfsound.com
   license:       Unlicense

END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_events/juce_events.h>
#include "include/Parameters.h"

#include <ranges>

#include "include/Distorter.h"
#include "include/JsonSerializer.h"
#include "include/PluginProcessor.h"
#include "include/PluginEditor.h"

// #include all additional header files below
