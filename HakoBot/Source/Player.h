/*********************************************************************
 * @file   Player.h
 * @brief  プレイヤーにつけるコンポーネントをまとめる
 *
 * @author 三品怜
 * @date   2025/11/18
 *********************************************************************/

#pragma once

#include <DXGameFrame.h>
#include "PlayerBlockHandler.h"

class Player : public Component
{
public:

	void Awake() override;
	PlayerBlockHandler* GetBlockHandler()
	{
		return m_pBlockHandler.Get();
	}

private:
	ObjPtr<PlayerBlockHandler> m_pBlockHandler;
};