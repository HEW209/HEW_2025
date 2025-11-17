/*********************************************************************
 * @file   GridField.h
 * @brief  グリッド土台のコンポーネント
 *
 * @author 芝晃佑
 * @date   2025/11/16
 *********************************************************************/


#pragma once

#include <optional>
#include <vector>

#include <DXGameFrame.h>

#include "GridBlockData.h"
#include "PlaceCursor.h"


 /**
  * @brief グリッド土台のコンポーネント
  */
class GridField : public Component
{
public:
	GridField();
	~GridField() = default;

	void Start() override;
	void OnDestroy() override;

	void SetSize(Vec3Int size);
	Vec3Int GetSize() const { return m_gridData.GetSize(); }

	// ブロックがグリッドに少しでも重なっているか
	bool IsOverlap(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation);

	// 配置カーソルをセット
	void SetPlaceCursor(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation);	

	void ResetPlaceCursor();

	// 配置カーソルの位置にブロックを配置
	// 配置できた場合trueを返す
	bool PlaceBlock();

	// 除外カーソルをセット
	void SetRemoveCursor(const Vector3& position);

	void ResetRemoveCursor();

	// 除外カーソルの位置からブロックを取り除く
	// 戻り値は取り除いたブロックのデータ
	// ブロックが存在しない場合はnulloptを返す
	std::optional<BlockSetAndRotationData> RemoveBlock();

	// クリア判定の形状を登録する関数
	void SetClearShape(DynamicDimArray<bool, 2> shapeX , DynamicDimArray<bool, 2> shapeY , DynamicDimArray<bool, 2> shapeZ);

	// クリア判定
	bool IsClear();

	//グリッド内判定
	bool IsInside(const Vector3& position);

private:
	bool IsInside(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation);
	bool CanPlace(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation);
	Vec3Int CalcGridCoord(const Vector3& position);
	Vector3 SnapGrid(const Vector3& position);
	Vector3 GetGridOrigin();
	bool IsOverlapBlockImpl(const Vec3& pos, const Vec3& start, const Vec3& end);
	bool IsInsideBlockImpl(const Vec3& pos, const Vec3& start, const Vec3& end);

	std::vector<ObjPtr<GameObject>> m_pPlacedBlocks;
	GridBlockData m_gridData;
	ObjPtr<GameObject> m_pPlaceCursor;
	ObjPtr<PlaceCursor> m_pPlaceCursorComponent;
	uint8_t m_removeCursorBlockId;
	DynamicDimArray<bool, 2> m_clearShape[3];
};