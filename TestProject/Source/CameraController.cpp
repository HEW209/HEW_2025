#include "CameraController.h"

CameraController::CameraController()
{
}

void CameraController::Update()
{
	Vector3 input = Vector3::zero;
	if (Input::GetKeyHold(KeyCode::D))
		input.x += 1.0f;
	if (Input::GetKeyHold(KeyCode::A))
		input.x -= 1.0f;
	if (Input::GetKeyHold(KeyCode::UP))
		input.y += 1.0f;
	if (Input::GetKeyHold(KeyCode::DOWN))
		input.y -= 1.0f;
	if (Input::GetKeyHold(KeyCode::W))
		input.z += 1.0f;
	if (Input::GetKeyHold(KeyCode::S))
		input.z -= 1.0f;

	float rotateInput = 0.0f;
	if (Input::GetKeyHold(KeyCode::E))
		rotateInput += 1.0f;
	if (Input::GetKeyHold(KeyCode::Q))
		rotateInput -= 1.0f;

	if (Input::GetKeyDown(KeyCode::I))
	{
		DirectionalLight::GetMain()->SetLightColor(Color(1.0f, 0.3f, 1.0f, 1.0f));
	}
	if (Input::GetKeyDown(KeyCode::O))
	{
		DirectionalLight::GetMain()->SetLightColor(Color(1.0f, 1.0f, 0.3f, 1.0f));
	}
	if (Input::GetKeyDown(KeyCode::P))
	{
		DirectionalLight::GetMain()->SetLightColor(Color(0.3f, 1.0f, 1.0f, 1.0f));
	}

	Vector3 move = input;
	move = Camera::GetMain()->GetTransform()->GetQuaternion() * move;
	move = move.Normalized();
	move *= 0.2f;

	GetTransform()->Translate(move);
	GetTransform()->Rotate(0.0f, rotateInput * 3.0f, 0.0f);
}
