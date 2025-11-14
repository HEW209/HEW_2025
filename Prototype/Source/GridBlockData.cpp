#include "GridBlockData.h"

GridBlockData::GridBlockData(size_t width, size_t height, size_t depth)
{
	m_size.x = width;
	m_size.y = height;
	m_size.z = depth;
	m_gridData.Resize(width, height, depth, 0u);
}

bool GridBlockData::CanPlace(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation)
{
	for (auto&& blockPos : blockSetData.blocks) {
		Vec3Int pos = rotation * blockPos + position;
		// 境界チェック
		for (int i = 0; i < 3; ++i) {
			if (pos[i] < 0 || m_size[i] <= pos[i]) {
				return false;
			}
		}
		// ブロックが存在するか
		if (m_gridData(pos.x, pos.y, pos.z)) {
			return false;
		}
	}

	return true;
}

bool GridBlockData::PlaceBlock(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation)
{
	// 置けるか確認
	if (!CanPlace(blockSetData, position, rotation)) {
		return true;
	}

	// ブロックID生成
	BlockIdType blockId = m_blockIdGen.Generate();
	if (m_blocks.size() < blockId) {
		m_blocks.resize(blockId);
	}

	// ブロックリストに追加
	BlockData blockData;
	blockData.blockSet = blockSetData;
	blockData.position = position;
	blockData.rotation = rotation;

	m_blocks[blockId - 1] = blockData;

	// 配置する座標にブロックIDを記録
	for (auto&& blockPos : blockSetData.blocks) {
		Vec3Int pos = rotation * blockPos + position;
		m_gridData(pos.x, pos.y, pos.z) = blockId;
	}
}

std::optional<BlockSetAndRotationData> GridBlockData::RemoveBlock(const Vec3Int position)
{
	// 境界チェック
	for (int i = 0; i < 3; ++i) {
		if (position[i] < 0 || m_size[i] <= position[i]) {
			return std::nullopt;
		}
	}

	// 配置されているブロックのIDを取得
	BlockIdType blockId = m_gridData(position.x, position.y, position.z);

	// ブロックが無ければnulloptを返す
	if (!blockId) {
		return std::nullopt;
	}

	BlockData blockData = m_blocks[blockId - 1];

	// 配置されているブロックの座標に無効値0uを記録
	for (auto&& blockPos : blockData.blockSet.blocks) {
		Vec3Int pos = blockData.rotation * blockPos + blockData.position;
		m_gridData(pos.x, pos.y, pos.z) = 0u;
	}

	m_blockIdGen.Release(blockId);

	BlockSetAndRotationData data;
	data.blockSet = blockData.blockSet;
	data.rotation = blockData.rotation;

	return data;
}

DynamicDimArray<bool, 2> GridBlockData::GetShape(int projectionAxis)
{
	if (projectionAxis < 0 || projectionAxis > 2) {
		assert(false && "Invalid projection axis");
		return DynamicDimArray<bool, 2>();
	}

	// 射影面を計算
	size_t axisU = (projectionAxis == 0) ? 1 : 0;
	size_t axisV = (projectionAxis == 2) ? 1 : 2;

	// グリッドサイズを取得
	size_t sizeU = static_cast<size_t>(m_size[axisU]);
	size_t sizeV = static_cast<size_t>(m_size[axisV]);
	size_t sizeProjected = static_cast<size_t>(m_size[projectionAxis]);

	DynamicDimArray<bool, 2> shape(sizeU, sizeV);

	size_t coords[3];

	// 形状を計算
	for (size_t v = 0; v < sizeV; ++v) {
		for (size_t u = 0; u < sizeU; ++u) {
			bool hasBlock = false;

			for (size_t p = 0; p < sizeProjected; ++p) {
				coords[axisU] = u;
				coords[axisV] = v;
				coords[projectionAxis] = p;

				// ブロックが存在するか
				if (m_gridData(coords[0], coords[1], coords[2]) != 0) {
					hasBlock = true;
					break;
				}
			}

			if (hasBlock) {
				shape(u, v) = true;
			}
		}
	}

	return shape;
}