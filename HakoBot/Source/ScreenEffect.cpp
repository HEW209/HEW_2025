#include "ScreenEffect.h"

ScreenEffect::ScreenEffect()
{
}

void ScreenEffect::Awake()
{
	for (int i = 0; i < 4; ++i)
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		obj->GetTransform()->SetEulerAngle(0.0f, 90.0f * i, 0.0f);
		auto effect = obj->AddComponent<EffectRenderer>();
		effect->Load("Assets/Effect/Screen/screen_effect.efkefc");
		effect->Play();
		m_effects[i] = effect;
	}
}

void ScreenEffect::Update()
{
}

void ScreenEffect::SetSize(float x, float z)
{
	for (int i = 0; i < 4; ++i)
	{
		if (i % 2 == 0)
		{
			m_effects[i]->GetTransform()->Translate(0.0f, 0.0f, -z * 0.5f, Space::LOCAL);
			m_effects[i]->GetTransform()->SetScale(Vector3(x, x, x) * 0.1f);
		}
		else
		{
			m_effects[i]->GetTransform()->Translate(0.0f, 0.0f, -x * 0.5f, Space::LOCAL);
			m_effects[i]->GetTransform()->SetScale(Vector3(z, z, z) * 0.1f);
		}
	}
}
