#pragma once


#include <DXGameFrame.h>

#include "BlockData.h"
#include "BlockObject.h"
#include "GridBlockData.h"
#include "GridField.h"
//プレイヤー移動用
#include "PlayerMove.h"

#include "PlayerBlockHandler.h"
class PlayerBlockDebugSceneManager : public Component
{
public:
	PlayerBlockDebugSceneManager();
	~PlayerBlockDebugSceneManager();

	void Start() override;
	void Update() override;

	//
	void SetGridField(GridField* pGridField);
	GridField* GetGridField() { return m_pGridField.Get(); }
	static PlayerBlockDebugSceneManager* GetInstance() { return s_pInstance; }

private:

	static PlayerBlockDebugSceneManager* s_pInstance;

	ObjPtr<GridField> m_pGridField;

};