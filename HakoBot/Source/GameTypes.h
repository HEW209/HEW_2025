#pragma once

#include "DynamicDimensionalArray.h"

using BlockIdType = uint8_t;
constexpr BlockIdType BLOCK_ID_EMPTY = 0u;

using ShapeType = DynamicDimArray<bool, 2>;