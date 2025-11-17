#pragma once
/*********************************************************************
 * @file   PlayerBlockHandler.h
 * @brief  プレイヤーブロックを操作するための
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

	
	void Start() override;
	void Update() override;

	void TryPlaceBlock();
	void SetBlockObject(BlockObject* pBlockObject);

private:

	//ブロックが置けるか判定する関数
	bool CheckPlaceBlock();

	//持ってるブロックオブジェクト
	ObjPtr<BlockObject> m_pBlockObject;
	
};