#include "GuideUIController2.h"
#include "Easing.h"

void GuideUIController2::Start()
{
	//‰ŠúêŠŽæ“¾
	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();
	Complete = false;
}

void GuideUIController2::Update()
{
	if (Input::GetKeyHold(KeyCode::D))
	{
		m_value += 0.1f;
		m_value2 += 0.1f;

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

	Vector3 offset = Vector3::zero;
	offset.x = Easing::OutQuad(-m_value, 5.0f, 0.8f, 0.0f);
	GetTransform()->SetPosition(m_defaultPosition + offset);

	//if (Complete)
	//{
	//	Vector3 offset2 = Vector3::zero;
	//	offset2.y = Easing::InSine(m_value2, 5.0f, -0.8f, 0.0f);
	//	GetTransform()->SetScale(m_defaultScale + offset);
	//}

}
