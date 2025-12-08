/*********************************************************************
 * @file   BlockObject.h
 * @brief  ブロックのコンポーネント
 *
 * @author 芝晃佑
 * @date   2025/11/15
 *********************************************************************/


#pragma once

#include <vector>

#include <DXGameFrame.h>

#include "BlockData.h"


/**
 * @brief ブロックのコンポーネント
 */
class BlockObject : public Component
{
public:
	BlockObject() :m_shouldUseCollider(true) {}
	~BlockObject() = default;

	void OnDestroy() override;

	void SetBlockSet(const BlockSetData& blockSet);
	const BlockSetData& GetBlockSet() const { return m_blockSet; }

	void SetSelect(bool value);

	/**
	 * @brief ブロックが地面に設置した際の原点のY軸オフセットを取得する
	 * @note 回転を考慮し、最も低いブロックの頂点がY=0になるようにするためのY軸オフセットを計算します。
	 * @return 地面に設置するためのY軸オフセット
	 */
	float GetGroundYOffset();

	/**
	 * @brief ブロックが地面に設置した際の原点のオフセットを取得する
	 * @note 回転を考慮し、最も低いブロックの頂点がY=0になるようにするためのオフセットを計算します。
	 * @return 地面に設置するためのオフセット (Vector3(0.0f, offset, 0.0f))
	 */
	Vector3 GetGroundOffset();

	/**
	 * @brief 指定したワールド座標がブロックの内側にあるか判定する
	 * @param worldPosition 判定したいワールド座標
	 * @return いずれかのブロックの内側にあれば true
	 */
	bool IsInside(const Vector3& worldPosition);

	void SetUseCollider(bool value) { m_shouldUseCollider = value; }

	Vector3 GetSize();

	Vector3 GetCenterOffset();

private:
	BlockSetData m_blockSet;
	std::vector<ObjPtr<GameObject>> m_pBlocks;
	Vector3 m_size;
	bool m_shouldUseCollider;
};