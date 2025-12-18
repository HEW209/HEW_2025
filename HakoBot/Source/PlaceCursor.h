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
	void SetModelPath(const std::string& modelPath);
	const std::string& GetModelPath() const { return m_modelPath; }
	const BlockSetData& GetBlockSet() const { return m_blockSet; }

	void SetPlaceable(bool value);

private:
	BlockSetData m_blockSet;
	std::string m_modelPath;

	/// ブロック情報
	struct BlockObj
	{
		ObjPtr<GameObject> pObj;
		ObjPtr<MeshRenderer> pRenderer;		// レンダラーもキャッシュ
	};

	std::vector<BlockObj> m_pBlocks;
};