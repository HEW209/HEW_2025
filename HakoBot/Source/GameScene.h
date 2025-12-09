#pragma once

#include <DXGameFrame.h>

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void Init() override;
	bool GetK();
	
private:
	bool K;
};