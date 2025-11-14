#pragma once

#include <vector>

#include <DXGameFrame.h>

#include "BlockData.h"


class BlockObject : public Component
{
public:
	BlockObject() = default;
	~BlockObject() = default;

	void OnDestroy() override;

	void SetBlockSet(BlockSetData& blockSet);
	const BlockSetData& GetBlockSet() const { return m_blockSet; }

private:
	BlockSetData m_blockSet;
	std::vector<ObjPtr<GameObject>> m_blocks;
};