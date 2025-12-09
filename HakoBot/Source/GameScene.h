#pragma once

#include <DXGameFrame.h>

// ƒTƒEƒ“ƒh
#include "SoundMaster.h"
#include "SoundData.h"
#include "SoundPlayer.h"

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

	WaveData bgm;
	SoundPlayer bgmPlayer;
};