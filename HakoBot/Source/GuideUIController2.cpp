#include "GuideUIController2.h"
#include "Easing.h"
#include "GameScene.h"

GameScene gamescene;

void GuideUIController2::Start()
{
	//‰ŠúêŠŽæ“¾
	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();
	Complete = false;
	m_value = 0;
}

void GuideUIController2::Update()
{
	if (Input::GetKeyHold(KeyCode::D))
	{
		m_value += 0.1f;

		if (m_value > 5.0f)
		{
			m_value = 5.0f;

			if (!Complete)
			{
				Complete = true;
			}
		}
	}
	else
	{
		m_value -= 0.1f;
		if (m_value < 0.0f)
		{
			m_value = 0.0f;
		}
	}

	if (gamescene.GetK())
	{
		Vector3 offset = Vector3::zero;
		offset.x = Easing::OutQuad(-m_value, 5.0f, 0.8f, 0.0f);
		GetTransform()->SetPosition(m_defaultPosition + offset);
	}
	else
	{
		Vector3 offset = Vector3::zero;
		offset.y = Easing::OutQuad(m_value, 5.0f, 1.5f, 0.0f);
		GetTransform()->SetPosition(m_defaultPosition + offset);
	}
	

}
