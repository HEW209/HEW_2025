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
	void SetPillarObject(GameObject* pPillar)
	{
		m_pillars.push_back(pPillar);
	}
	void SetPlayerCollider(Collider* pCollider)
	{
		m_pCollider = pCollider;
	}

private:

	float m_targetY;
	bool m_targetLock;
	std::vector<GameObject*> m_pillars;
	ObjPtr<Collider> m_pCollider;
};