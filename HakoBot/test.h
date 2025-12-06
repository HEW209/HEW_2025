#pragma once
#include <DXGameFrame.h>

class test : public Component
{
public:
	void Update() override;

	struct HologramParam
	{
		Color baseColor = { 0.0f,0.2f,1.0f,1.0f };
		float emiStrength = 1.0f;
		float time = 0.0f;
	};

	HologramParam m_hologramParam;
private:


};