#pragma once

#include <DXGameFrame.h>

class SampleCamera : public Component
{
public:
	SampleCamera();
	~SampleCamera() = default;

	void Update() override;

private:
	float m_cameraAngle;
	float m_cameraDistance;
	float m_rotateSpeed;
};