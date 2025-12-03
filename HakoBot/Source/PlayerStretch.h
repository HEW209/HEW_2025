/*********************************************************************
 * @file   PlayerStretch.h
 * @brief  プレイヤーを伸び縮みさせる	PlayerHeadにつけてね
 *
 * @author 三品怜
 * @date   2025/11/19
 *********************************************************************/
#pragma once

#include <DXGameFrame.h>

class PlayerStretch : public Component
{
public:

	PlayerStretch();
	void Update() override;
	void SetPillarObject(GameObject* pPillar) { m_pillar = pPillar; }

private:

	float m_targetY;
	GameObject* m_pillar;
};