#pragma once
#include "Common.h"

namespace Param {
// constexpr accesors for the param ranges
frange_t speedRange();
frange_t depthRange();
juce::StringArray phasingTypes();
apvts::ParameterLayout getLayout();
}  // namespace Param
