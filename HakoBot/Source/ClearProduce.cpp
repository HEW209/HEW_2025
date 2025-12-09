#include "ClearProduce.h"

ClearProduce::ClearProduce():
	m_isActive(false),
	m_pCamera(nullptr),
	m_state(MAE),
	m_count(0)
{
}

void ClearProduce::Start()
{
	m_pCamera = GetGameObject()->AddComponent<Camera>();
	Camera::Config cameraConfig;
	cameraConfig.fovAngle = 30.0f;
	m_pCamera->SetConfig(cameraConfig);
	// ‰ŠúˆÊ’u
	GetTransform()->SetPosition(0.0f, 2.0f, -10.0f);
}



void ClearProduce::Update()
{
	if (!m_isActive)
	{
		if (Input::GetKeyDown(KeyCode::ENTER))
		{
			m_pCamera->SetMain();
			m_isActive = true;
		}
		return;
	}

	switch (m_state)
	{
	case MAE:
		m_count++;

		if (60 < m_count)
		{
			m_count = 0;
			m_state = MIGI;
			GetTransform()->SetPosition(10.0f, 2.0f, 0.0f);
			GetTransform()->SetEulerAngle(0.0f, -90.0f, 0.0f);
		}

		break;

	case MIGI:
		m_count++;

		if (60 < m_count)
		{
			m_count = 0;
			m_state = UE;
			GetTransform()->SetPosition(0.0f, 10.0f, 0.0f);
			GetTransform()->SetEulerAngle(90.0f, 0.0f, 0.0f);
		}

		break;

	case UE:
		m_count++;

		if (60 < m_count)
		{
			m_count = 0;
			m_state = FINAL;
			GetTransform()->SetPosition(8.0f, 1.0f, -8.0f);
			GetTransform()->SetEulerAngle(-8.0f, 315.0f, 0.0f);
		}

		break;

	case FINAL:
		break;

	}
}
