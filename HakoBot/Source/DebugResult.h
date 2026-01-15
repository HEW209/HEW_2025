//DebugResult.h
#pragma once
//Pキーでリザルト表示したいけど、GameSceneにUpdateがないので作成

#include <DXGameFrame.h>
#include "GameFrame/Component.h"

class DebugResult : public Component
{
	void Start() override;
	void Update() override;
	void CreateResult();
};