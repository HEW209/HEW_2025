#include "TransformGUI.h"
#include <../imgui/imgui.h>

TransformGUI::TransformGUI() :
	m_name("No Name")
{
}

void TransformGUI::Update()
{
	Vector3 pos = GetTransform()->GetPosition();
	Vector3 angle = GetTransform()->GetEulerAngle();

	ImGui::Begin("Transform");
	ImGui::Text("%s", m_name.c_str());
	ImGui::Text("Position x : %f", pos.x);
	ImGui::Text("Position y : %f", pos.y);
	ImGui::Text("Position z : %f", pos.z);
	ImGui::Text("Rotation x : %f", angle.x);
	ImGui::Text("Rotation y : %f", angle.y);
	ImGui::Text("Rotation z : %f", angle.z);
	ImGui::End();
}
