#include "TestComponent.h"
#include <../imgui/imgui.h>

TestComponent::TestComponent() :
	m_transMode(TransformMode::MOVE),
	m_space(Space::WORLD)
{
}

void TestComponent::Update()
{
	if (Input::GetKeyDown(KeyCode::W))
	{
		m_transMode = TransformMode::MOVE;
	}
	else if(Input::GetKeyDown(KeyCode::E))
	{
		m_transMode = TransformMode::ROTATE;
	}

	if (Input::GetKeyDown(KeyCode::SHIFT))
	{
		if (m_space == Space::WORLD)
		{
			m_space = Space::LOCAL;
		}
		else
		{
			m_space = Space::WORLD;
		}
	}

	Vector2 input = Vector2::zero;
	if (Input::GetKeyHold(KeyCode::RIGHT))
		input.x += 1.0f;
	if (Input::GetKeyHold(KeyCode::LEFT))
		input.x -= 1.0f;
	if (Input::GetKeyHold(KeyCode::UP))
		input.y += 1.0f;
	if (Input::GetKeyHold(KeyCode::DOWN))
		input.y -= 1.0f;

	Transform* transform = GetTransform();

	if (m_transMode == TransformMode::MOVE)
	{
		transform->Translate(input.x * 0.2f, input.y * 0.2f, 0.0f, m_space);
	}
	if (m_transMode == TransformMode::ROTATE)
	{
		transform->Rotate(input.y * 5.0f, input.x * 5.0f, 0.0f, m_space);
	}

	Vector3 wPos = transform->GetPosition(Space::WORLD);
	Vector3 lPos = transform->GetPosition(Space::LOCAL);
	Vector3 scale = transform->GetScale();
	Vector3 wAngle = transform->GetEulerAngle(Space::WORLD);
	Vector3 lAngle = transform->GetEulerAngle(Space::LOCAL);

	ImGui::Begin("test");
	if (m_transMode == TransformMode::MOVE)
	{
		ImGui::Text("Mode : Move");
	}
	else
	{
		ImGui::Text("Mode : Rotate");
	}
	if (m_space == Space::WORLD)
	{
		ImGui::Text("Space : World");
		ImGui::Text("World x : %f", wPos.x);
		ImGui::Text("World y : %f", wPos.y);
		ImGui::Text("World z : %f", wPos.z);
	}
	else
	{
		ImGui::Text("Space : Local");
		ImGui::Text("Local x : %f", lPos.x);
		ImGui::Text("Local y : %f", lPos.y);
		ImGui::Text("Local z : %f", lPos.z);
	}
	ImGui::End();
}