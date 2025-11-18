/*********************************************************************
 * @file   GridBlockData.h
 * @brief  グリッド上のブロックデータを管理するクラス
 *
 * @author 芝晃佑
 * @date   2025/11/15
 *********************************************************************/

#pragma once

#include <optional>

#include <DXGameFrame.h>

#include "BlockData.h"
#include "DynamicDimensionalArray.h"
#include "GameTypes.h"
#include "IDGenerator.h"
#include "Vec.h"


/**
 * @brief グリッド上のブロックデータを管理するクラス
 */
class GridBlockData
{
public:
	GridBlockData(size_t width, size_t height, size_t depth);
	GridBlockData(Vec3Int size) : GridBlockData(size.x, size.y, size.z) {}
	GridBlockData() : GridBlockData(0, 0, 0) {}

	Vec3Int GetSize() const { return m_size; }
	BlockIdType GetId(const Vec3Int& position) const;

	// ブロックを置ける場合はtrueを返す
	bool CanPlace(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation) const;

	// ブロックを置く。戻り値は置いたブロックのId。置けない場合は0を返す
	BlockIdType PlaceBlock(const BlockSetData& blockSetData, const Vec3Int& position, const Quaternion& rotation);

	// ブロックを取り除く。戻り値は取り除いたブロックのId。ブロックが存在しない場合は0を返す
	BlockIdType RemoveBlock(const Vec3Int position);

	std::optional<BlockSetAndRotationData> RemoveBlock(BlockIdType blockId);

	// 二次元の形状（シルエット）を取得
	// projectionAxis: 0=X, 1=Y, 2=Z
	ShapeType GetShape(int projectionAxis) const;
	ShapeType GetShapeX() const { return GetShape(0); }
	ShapeType GetShapeY() const { return GetShape(1); }
	ShapeType GetShapeZ() const { return GetShape(2); }

private:
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