#pragma once


#include <DXGameFrame.h>

#include "BlockData.h"
#include "BlockObject.h"
#include "GridBlockData.h"
#include "GridField.h"
//プレイヤー移動用
#include "PlayerMove.h"

#include "PlayerBlockHandler.h"
#include "Player.h"
#include "GuideUIMenuController.h"
#include "Tutorial.h"

class GameState : public Component
{
public:
	GameState();
	~GameState();

	void Start() override;
	void Update() override;

	void SetLevelName(const std::string& levelName);
	std::string GetLevelName();

	void SetGridField(GridField* pGridField);
	GridField* GetGridField() { return m_pGridField.Get(); }
	void AppendWorldBlock(BlockObject* pBlockObject);
	void RemoveWorldBlock(BlockObject* pBlockObject);
	const std::vector<ObjPtr<BlockObject>>& GetWorldBlocks() { return m_pWorldBlocks; }
	void SetPlayer(Player* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	Player* GetPlayer()
	{
		return m_pPlayer.Get();
	}
	bool IsBlockTransparent();

	bool IsClear();

	// クリア演出中
	bool IsClearEnter();

	static GameState* GetInstance() { return s_pInstance; }
	static void SetCurrentStegaNo(int no) { s_currentStageNo = no; }
	static int GetCurrentStegaNo() { return s_currentStageNo; };

	void SetStageSize(const Vector3& size) { m_stageSize = size; }
	Vector3 GetStageSize() const { return m_stageSize; }
	void SetStagePos(const Vector3& pos) { m_stagePos = pos; }
	Vector3 GetStagePos() const { return m_stagePos; }

	void SetTutorial(ITutorial* pTutorial) { m_pTutorial = pTutorial; }
	ITutorial* GetTutorial() { return m_pTutorial.Get(); }

private:
	
	static GameState* s_pInstance;
	static int s_currentStageNo;
	ObjPtr<GridField> m_pGridField;
	//ワールド空間（グリッド外）に存在するすべてのブロック
	std::vector<ObjPtr<BlockObject>> m_pWorldBlocks;
	
	ObjPtr<Player> m_pPlayer;
	std::string m_levelName;
	bool m_isBlockTransparent;
	Vector3 m_stageSize;
	Vector3 m_stagePos;
	bool m_isClearEnter;

	// デバッグ用
	bool m_colorBalanceMode;

	ObjPtr<ITutorial> m_pTutorial;
};