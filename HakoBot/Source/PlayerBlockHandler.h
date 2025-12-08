#pragma once
/*********************************************************************
 * @file   PlayerBlockHandler.h
 * @brief  プレイヤーブロックを操作するためのコンポーネントクラス
 *
 * @author 長尾潤弥
 * @date   2025/11/17
 *********************************************************************/


#include <DXGameFrame.h>
#include "BlockObject.h"
#include "GridField.h"

class PlayerBlockHandler : public Component
{
public:
	PlayerBlockHandler();
	
	void Awake() override;
	void Update() override;

private:
	void SetBlockSet(const BlockSetData& blockSet);


	//持ってるブロックオブジェクト
	ObjPtr<BlockObject> m_pBlockObject;

	//ブロックの回転情報を持つ、親オブジェクト
	ObjPtr<GameObject> m_pBlockHolder;
};