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

	// 二次元の形状（シルエット）を取得
	// projectionAxis: 0=X, 1=Y, 2=Z
	DynamicDimArray<bool, 2> GetShape(int projectionAxis);
	DynamicDimArray<bool, 2> GetShapeX() { return GetShape(0); }
	DynamicDimArray<bool, 2> GetShapeY() { return GetShape(1); }
	DynamicDimArray<bool, 2> GetShapeZ() { return GetShape(2); }

private:
	using BlockIdType = uint8_t;

	struct BlockData
	{
		BlockSetData	blockSet;
		Vec3Int			position;
		Quaternion		rotation;
	};

	Vec3Int m_size;
	DynamicDimArray<BlockIdType, 3> m_gridData;
	std::vector<BlockData> m_blocks;
	IDGenerator<BlockIdType> m_blockIdGen;
};