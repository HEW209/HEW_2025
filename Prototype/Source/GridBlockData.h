#pragma once

#include <DXGameFrame.h>

#include "BlockSetData.h"
#include "DynamicArray3D.h"
#include "Vec.h"


class GridBlockData
{
public:
	GridBlockData(size_t width, size_t height, size_t depth);

	bool CanPlace(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation);
	bool PlaceBlock();

private:
	Vec3Int m_size;
	DynamicArray3D<bool> m_blockData;
};