#pragma once

#include <DXGameFrame.h>
#include"StageSelectKeyEnter.h"

class StageSelectScene : public Scene
{
public:
	StageSelectScene() = default;
	~StageSelectScene() = default;

	void Init() override;
private:
	GameObject* aa;
};