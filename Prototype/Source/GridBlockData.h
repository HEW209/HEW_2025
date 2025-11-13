#pragma once

#include <DXGameFrame.h>

#include "BlockSetData.h"
#include "DynamicArray3D.h"
#include "Vec.h"


class GridBlockData
{
public:
	bool CanPlace(const BlockSetData& blockSetData, Vec3Int position, Quaternion rotation);

	bool PlaceBlockSet();

private:
	DynamicArray3D<bool> m_blockData;
};