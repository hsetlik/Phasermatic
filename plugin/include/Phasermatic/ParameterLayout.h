#pragma once
#include "Common.h"

namespace Param {
// accesors for the param ranges
frange_t speedRange();
frange_t depthRange();
apvts::ParameterLayout getLayout();
}  // namespace Param
