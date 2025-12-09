#pragma once

#include <DXGameFrame.h>

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void Init() override;

private:
	void KeyBind();
	void CreateGridField();
	void CreateStageSet();

};