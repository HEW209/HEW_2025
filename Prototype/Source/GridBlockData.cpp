#include "GridBlockData.h"

GridBlockData::GridBlockData(size_t width, size_t height, size_t depth)
{
	m_size.x = width;
	m_size.y = height;
	m_size.z = depth;
	m_blockData.Resize(width, height, depth);
}

bool GridBlockData::CanPlace(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation)
{
	for (auto&& blockPos : blockSetData.blocks) {
		Vec3Int pos = rotation * blockPos + position;
		for (int i = 0; i < 3; ++i) {
			if (pos[i] < 0 || m_size[i] <= pos[i]) {
				return false;
			}
		}
		if (m_blockData(pos.x, pos.y, pos.z)) {
			return false;
		}
	}

	return true;
}

bool GridBlockData::PlaceBlock()
{
	return false;
}
