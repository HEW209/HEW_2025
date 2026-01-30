// ScreenEffect.h
#pragma once
#include <DXGameFrame.h>

class ScreenEffect : public Component
{
public:
	ScreenEffect();

	void Awake() override;
	void Update() override;

	void SetSize(float x, float z);

private:
	ObjPtr<EffectRenderer> m_effects[4];
};
