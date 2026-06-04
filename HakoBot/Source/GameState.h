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

	// レベル名
	void SetLevelName(const std::string& levelName);
	std::string GetLevelName();

	// グリッドフィールド
	void SetGridField(GridField* pGridField);
	GridField* GetGridField() { return m_pGridField.Get(); }

	// ブロック
	void AppendWorldBlock(BlockObject* pBlockObject);
	void RemoveWorldBlock(BlockObject* pBlockObject);
	const std::vector<ObjPtr<BlockObject>>& GetWorldBlocks() { return m_pWorldBlocks; }
	
	// プレイヤー
	void SetPlayer(Player* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	Player* GetPlayer()
	{
		return m_pPlayer.Get();
	}

	// ブロック透明化
	bool IsBlockTransparent();

	// クリア状態
	bool IsClear();			// クリア条件を満たしているか
	bool IsClearEnter();	// クリアしているか

	// ステージ地形情報
	void SetStageSize(const Vector3& size) { m_stageSize = size; }
	Vector3 GetStageSize() const { return m_stageSize; }
	void SetStagePos(const Vector3& pos) { m_stagePos = pos; }
	Vector3 GetStagePos() const { return m_stagePos; }

	// チュートリアル
	void SetTutorial(ITutorial* pTutorial) { m_pTutorial = pTutorial; }
	ITutorial* GetTutorial() { return m_pTutorial.Get(); }

	// タイム
	float GetGameTime();
	void StartTimer();
	void StopTimer();

private:
	ObjPtr<GridField> m_pGridField;
	//ワールド空間（グリッド外）に存在するすべてのブロック
	std::vector<ObjPtr<BlockObject>> m_pWorldBlocks;
	
	ObjPtr<Player> m_pPlayer;
	std::string m_levelName;
	bool m_isBlockTransparent;
	Vector3 m_stageSize;
	Vector3 m_stagePos;
	bool m_isClearEnter;
	bool m_timerActive;
	float m_gameTime;
	ObjPtr<ITutorial> m_pTutorial;

	// デバッグ用
	bool m_colorBalanceMode;

public:
	static GameState* GetInstance() { return s_pInstance; }

	// ステージ番号
	static void SetCurrentStegaNo(int no) { s_currentStageNo = no; }
	static int GetCurrentStegaNo() { return s_currentStageNo; };

private:
	static GameState* s_pInstance;
	static int s_currentStageNo;
};