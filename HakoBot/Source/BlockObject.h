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
#include "OutlineMeshRenderer.h"


/**
 * @brief ブロックのコンポーネント
 */
class BlockObject : public Component
{
public:
	BlockObject() :m_shouldUseCollider(true) {}
	~BlockObject() = default;

	void Awake() override;
	void OnDestroy() override;

	void SetBlockSet(const BlockSetData& blockSet);
	const BlockSetData& GetBlockSet() const { return m_blockSet; }
	void SetModel(const std::string& modelPath);
	const std::string& GetModelPath() const { return m_modelPath; }

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

	Vector3 GetSize() const { return m_size; }

	Vector3 GetCenterOffset() const { return m_center * -1.0f; }

	Vector3 GetCenterGroundOffset()
	{
		Vector3 offset = m_center * -1.0f;
		offset.y = GetGroundYOffset();
		return offset;
	}

private:
	BlockSetData m_blockSet;
	std::vector<ObjPtr<GameObject>> m_pBlocks;
	ObjPtr<OutlineMeshRenderer> m_pBlockMeshRenderer;
	std::string m_modelPath;
	Vector3 m_size;
	Vector3 m_center;
	bool m_shouldUseCollider;
};