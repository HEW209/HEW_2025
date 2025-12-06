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

	//ブロックを置くための関数
	void SetBlockObject(BlockObject* pBlockObject);

private:
	//回転軸オブジェクト
	ObjPtr<GameObject> m_rotateRoot;

	//持ってるブロックオブジェクト
	ObjPtr<BlockObject> m_pBlockObject;
};