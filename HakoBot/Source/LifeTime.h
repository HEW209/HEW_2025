// LifeTime.h
#pragma once
#include <DXGameFrame.h>

class LifeTime : public Component
{
public:
	LifeTime();

	void Update() override;
	void SetLifeTime(float lifeTime)
	{
		m_lifeTime = lifeTime;
	}

private:
	float m_lifeTime;
};
