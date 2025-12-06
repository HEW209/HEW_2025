#include "SampleCameraController.h"

void SampleCameraController::Update()
{
	if (Input::GetKeyHold(KeyCode::MOUSE_LEFT))
	{
		GetTransform()->Rotate(0.0f, 5.0f, 0.0f);
	}
	if (Input::GetKeyHold(KeyCode::MOUSE_RIGHT))
	{
		GetTransform()->Rotate(0.0f, -5.0f, 0.0f);
	}
}
