#pragma once

#include <optional>

#include <DXGameFrame.h>

#include "BlockSetData.h"
#include "DynamicArray3D.h"
#include "IDGenerator.h"
#include "Vec.h"


class GridBlockData
{
public:
	GridBlockData(size_t width, size_t height, size_t depth);

	// ブロックを置ける場合はtrueを返す
	bool CanPlace(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation);

	// ブロックを置く。置けない場合はtrueを返す
	bool PlaceBlock(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation);

	// ブロックを取り除く。戻り値は取り除いたブロックのデータを格納したoptional<BlockSetData>。ブロックが存在しない場合はnulloptを返す
	std::optional<BlockSetData> RemoveBlock(const Vec3Int position);

private:
	using blockIdType = uint8_t;

	struct BlockData
	{
		BlockSetData	blockSet;
		Vec3Int			pos;
		Quaternion		rotation;
	};

	Vec3Int m_size;
	DynamicArray3D<blockIdType> m_gridBlockData;
	std::vector<BlockData> m_blocks;
	IDGenerator<blockIdType> m_blockIdGen;
};