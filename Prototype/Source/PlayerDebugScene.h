/*********************************************************************
 * @file   PlayerModelDebugScene.h
 * @brief  プレイヤーモデルのデバッグ用シーン
 *
 * @author 三品怜
 * @date   2025/11/18
 *********************************************************************/
#pragma once

#include <DXGameFrame.h>

class PlayerDebugScene : public Scene
{
public:
	void Init() final;

private:

	void CreatePlayer();
};