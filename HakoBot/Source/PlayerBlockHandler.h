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
	
	void Start() override;
	void Update() override;

	void TryPlaceBlock();
	//ブロックを置くための関数
	void SetBlockObject(BlockObject* pBlockObject);

private:

	//ブロックが置けるか判定する関数
	bool CheckPlaceBlock();

	//持ってるブロックオブジェクト
	ObjPtr<BlockObject> m_pBlockObject;
};