#pragma once

#include <DXGameFrame.h>

class SampleCamera : public Component
{
public:
	SampleCamera();
	~SampleCamera() = default;

	void Start() override;
	void Update() override;

	void SetTarget(Transform* target);

private:
	float m_cameraAngle;
	float m_cameraDistance;
	float m_rotateSpeed;

	Transform* m_target = nullptr;
};