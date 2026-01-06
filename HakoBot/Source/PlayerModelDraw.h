#pragma once
#include <DXGameFrame.h>
#include "PlayerBlockHandler.h"



class PlayerModelDraw : public Component
{
public:

	void Awake() override;
	void Update() override;
private:
	std::vector<std::string> blockFileNames = {
		"1masu",
		"L1",
		"tate2",
		"tate3",
		"yoko2",
		"yoko3",
	};
	//持ってるブロックオブジェクト
	ObjPtr<BlockObject> m_pBlockObject;
	
	//ブロックの回転情報を持つ、親オブジェクト
	ObjPtr<GameObject> m_pBlockHolder;
};
