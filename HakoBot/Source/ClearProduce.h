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

	enum State
	{
		MAE,
		MIGI,
		UE,
		FINAL
	};

	ClearProduce();
	void Start() override;
	void Update() override;

private:

	State m_state;
	bool m_isActive;
	Camera* m_pCamera;
	int m_count;
};