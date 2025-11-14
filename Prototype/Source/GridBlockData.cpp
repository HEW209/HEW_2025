#include "GridBlockData.h"

GridBlockData::GridBlockData(size_t width, size_t height, size_t depth)
{
	m_size.x = width;
	m_size.y = height;
	m_size.z = depth;
	m_gridBlockData.Resize(width, height, depth, 0u);
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
		if (m_gridBlockData(pos.x, pos.y, pos.z)) {
			return false;
		}
	}

	return true;
}

bool GridBlockData::PlaceBlock(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation)
{
	if (!CanPlace(blockSetData, position, rotation)) {
		return true;
	}

	blockIdType blockId = m_blockIdGen.Generate();
	if (m_blocks.size() < blockId) {
		m_blocks.resize(blockId);
	}

	BlockData blockData;
	blockData.blockSet = blockSetData;
	blockData.pos = position;
	blockData.rotation = rotation;

	m_blocks[blockId - 1] = blockData;

	for (auto&& blockPos : blockSetData.blocks) {
		Vec3Int pos = rotation * blockPos + position;
		m_gridBlockData(pos.x, pos.y, pos.z) = blockId;
	}
}

std::optional<BlockSetData> GridBlockData::RemoveBlock(const Vec3Int position)
{
	return std::optional<BlockSetData>();
}
