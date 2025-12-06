#include "test.h"

void test::Update()
{
	if (Input::GetKeyHold(KeyCode::A))
	{
		m_hologramParam.emiStrength += 0.05;
	}
	if (Input::GetKeyHold(KeyCode::D))
	{
		m_hologramParam.emiStrength -= 0.05;
	}

	if (Input::GetKeyDown(KeyCode::I))
	{
		m_hologramParam.baseColor = Color(1.0f, 0.1f, 0.1f, 1.0f);
	}
	if (Input::GetKeyDown(KeyCode::O))
	{
		m_hologramParam.baseColor = Color(0.0f, 1.0f, 0.1f, 1.0f);
	}
	if (Input::GetKeyDown(KeyCode::P))
	{
		m_hologramParam.baseColor = Color(0.0f, 0.2f, 1.0f, 1.0f);
	}

	m_hologramParam.time += 1.0f / 60.0f;

	MeshRenderer* renderer = GetGameObject()->GetComponent<MeshRenderer>();
	renderer->GetMaterial(0)->SetParameter(&m_hologramParam, sizeof(m_hologramParam));
}
