/*********************************************************************
 * @file   PlaceCursor.h
 * @brief  配置カーソルのコンポーネント
 *
 * @author 芝晃佑
 * @date   2025/11/16
 *********************************************************************/


#pragma once

#include <vector>

#include <DXGameFrame.h>

#include "BlockData.h"


 /**
  * @brief 配置カーソルのコンポーネント
  */
class PlaceCursor : public Component
{
public:
	PlaceCursor() = default;
	~PlaceCursor() = default;

	void OnDestroy() override;

	void SetBlockSet(const BlockSetData& blockSet);
	const BlockSetData& GetBlockSet() const { return m_blockSet; }

	void SetPlaceable(bool value);

private:
	BlockSetData m_blockSet;

	/// ブロック情報
	struct BlockObj
	{
		ObjPtr<GameObject> pObj;
		ObjPtr<MeshRenderer> pRenderer;		// レンダラーもキャッシュ
	};

	std::vector<BlockObj> m_pBlocks;
};