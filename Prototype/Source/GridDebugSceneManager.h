#pragma once

#include <vector>

#include <DXGameFrame.h>

#include "BlockData.h"
#include "BlockObject.h"
#include "GridBlockData.h"
#include "Vec.h"


class GridDebugSceneManager : public Component
{
public:
	GridDebugSceneManager();
	~GridDebugSceneManager() = default;

	void Start() override;
	void Update() override;

private:
	void PlaceBlock(Vec3Int position);
	void RemoveBlock(Vec3Int position);

	const Vector3 currentBlockPos{ 10.0f, 0.0f, 0.0f };
	const Vector3 gridFieldPos{ -10.0f, 0.0f, -1.0f };

	ObjPtr<GameObject> m_currentBlock;
	ObjPtr<BlockObject> m_currentBlockComponent;
	std::vector<ObjPtr<GameObject>> m_placedBlocks;
	GridBlockData m_gridData;
};