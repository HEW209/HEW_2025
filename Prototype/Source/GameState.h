#pragma once


#include <DXGameFrame.h>

#include "BlockData.h"
#include "BlockObject.h"
#include "GridBlockData.h"
#include "GridField.h"
//プレイヤー移動用
#include "PlayerMove.h"

#include "PlayerBlockHandler.h"
class GameState : public Component
{
public:
	GameState();
	~GameState();

	void Start() override;
	void Update() override;


	void SetGridField(GridField* pGridField);
	GridField* GetGridField() { return m_pGridField.Get(); }
	void AppendWorldBlock(BlockObject* pBlockObject);
	void RemoveWorldBlock(BlockObject* pBlockObject);
	const std::vector<ObjPtr<BlockObject>>& GetWorldBlocks() { return m_pWorldBlocks; }

	
	static GameState* GetInstance() { return s_pInstance; }
	
private:
	
	static GameState* s_pInstance;
	ObjPtr<GridField> m_pGridField;
	//ワールド空間（グリッド外）に存在するすべてのブロック
	std::vector<ObjPtr<BlockObject>> m_pWorldBlocks;
	
};