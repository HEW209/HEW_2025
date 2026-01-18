#include "LifeTime.h"

LifeTime::LifeTime() :
	m_lifeTime(0.0f)
{
}

void LifeTime::Update()
{
	m_lifeTime -= Time::GetDeltaTime();
	if (m_lifeTime <= 0.0f)
		GetGameObject()->Destroy();
}
