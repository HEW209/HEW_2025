/*********************************************************************
 * @file   ClearProduce.h
 * @brief  クリア演出
 *
 * @author 三品怜
 * @date   2025/12/09
 *********************************************************************/
#pragma once

#include <DXGameFrame.h>

class ClearProduce : public Component
{
public:

	ClearProduce();
	void Start() override;
	void Update() override;

private:

	bool m_isActive;
	Camera* m_pCamera;
	std::array<ObjPtr<EffectRenderer>, 2> m_effectRenderers;
	int m_count;

	float m_startCameraDistance;
	float m_midCameraDistance;
	float m_endCameraDistance;
};