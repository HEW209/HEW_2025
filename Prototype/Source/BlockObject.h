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
	BlockObject() = default;
	~BlockObject() = default;

	void OnDestroy() override;

	void SetBlockSet(const BlockSetData& blockSet);
	const BlockSetData& GetBlockSet() const { return m_blockSet; }

	void SetSelect(bool value);

private:
	BlockSetData m_blockSet;
	std::vector<ObjPtr<GameObject>> m_pBlocks;
};