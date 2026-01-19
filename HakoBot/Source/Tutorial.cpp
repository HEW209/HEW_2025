#include "Tutorial.h"
#include "VecUtil.h"
#include "Easing.h"
#include "InputManager.h"

ITutorial::~ITutorial()
{
}


Tutorial1::Tutorial1()
	: m_finished(false)
	, m_cameraMoveDuration(0.0f)
	, m_cameraMoveTimer(0.0f)
	, m_step(0)
{
}

void Tutorial1::Awake()
{
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetTransform()->SetParent(GetTransform());
		m_pCamera = obj->AddComponent<Camera>();
		Camera::Config cameraConfig;
		cameraConfig.fovAngle = 30.0f;
		m_pCamera->SetConfig(cameraConfig);
	}
	m_pLine = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pLine->LoadTexture("Assets/Textures/Tutorial/tutorial1_line.png", false);
	m_pLine->SetSize(1280.0f);
	m_pLine->SetUI(true);
	m_pLine->SetEnabled(false);
	m_pText1 = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pText1->LoadTexture("Assets/Textures/Tutorial/tutorial1_text1.png", false);
	m_pText1->SetSize(1280.0f);
	m_pText1->SetUI(true);
	m_pText1->SetEnabled(false);
	m_pText2 = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pText2->LoadTexture("Assets/Textures/Tutorial/tutorial1_text2.png", false);
	m_pText2->SetSize(1280.0f);
	m_pText2->SetUI(true);
	m_pText2->SetEnabled(false);
}

void Tutorial1::Start()
{
}

void Tutorial1::LateUpdate()
{
	switch (m_step)
	{
	case 0:
	{
		auto transform = m_pCamera->GetTransform();
		m_pDefaultCamera = Camera::GetMain();
		auto mainCamTransform = m_pDefaultCamera->GetTransform();
		m_prevCameraPos = mainCamTransform->GetPosition();
		m_prevCameraRot = mainCamTransform->GetQuaternion();
		transform->SetPosition(m_prevCameraPos);
		transform->SetQuaternion(m_prevCameraRot);
		m_nextCameraPos = Vector3(-12.0f, 12.3f, -12.0f);
		m_nextCameraRot = Quaternion::Euler(30.0f, 54.0f, 0.0f);
		m_cameraMoveDuration = 2.0f;
		m_cameraMoveTimer = 0.0f;
		m_pCamera->SetMain();

		InputManager::ChangeBindType(InputBindType::UI);
		++m_step;
	}
	break;

	case 1:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			m_pLine->SetEnabled(true);
			if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
				++m_step;
				auto transform = m_pCamera->GetTransform();
				m_prevCameraPos = transform->GetPosition();
				m_prevCameraRot = transform->GetQuaternion();
				m_nextCameraPos = Vector3(-5.0f, 4.0f, -5.0f);
				m_nextCameraRot = Quaternion::Euler(30.0f, 300.0f, 0.0f);
				m_cameraMoveDuration = 2.0f;
				m_cameraMoveTimer = 0.0f;
				m_pLine->SetEnabled(false);
			}
		}
		else {
			{
				float rate = Math::Clamp01((m_cameraMoveTimer - 1.5f) * 2.0f);
				float t = Easing::OutBack(rate, 1.0f, 1.7f);
				m_pText1->SetOffsetPos(0.0f, std::lerp(-7.0f, 0.0f, t));
				m_pText1->SetEnabled(true);
			}
		}
	}
	break;

	case 2:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
				++m_step;
				auto transform = m_pCamera->GetTransform();
				m_prevCameraPos = transform->GetPosition();
				m_prevCameraRot = transform->GetQuaternion();
				m_nextCameraPos = m_pDefaultCamera->GetTransform()->GetPosition();
				m_nextCameraRot = m_pDefaultCamera->GetTransform()->GetQuaternion();
				m_cameraMoveDuration = 2.0f;
				m_cameraMoveTimer = 0.0f;
			}
		}
		else {
			{
				float rate = Math::Clamp01((m_cameraMoveTimer) * 3.0f);
				if (rate < 1.0f) {
					float t = Easing::InQuad(rate, 1.0f);
					m_pText1->SetOffsetPos(0.0f, std::lerp(0.0f, 7.0f, t));
				}
				else {
					m_pText1->SetEnabled(false);
				}
			}
			{
				float rate = Math::Clamp01((m_cameraMoveTimer - 1.5f) * 2.0f);
				float t = Easing::OutBack(rate, 1.0f, 1.7f);
				m_pText2->SetOffsetPos(0.0f, std::lerp(-7.0f, 0.0f, t));
				m_pText2->SetEnabled(true);
			}
		}
	}
	break;

	case 3:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			++m_step;
		}
		else {
			{
				float rate = Math::Clamp01((m_cameraMoveTimer) * 3.0f);
				if (rate < 1.0f) {
					float t = Easing::InQuad(rate, 1.0f);
					m_pText2->SetOffsetPos(0.0f, std::lerp(0.0f, 7.0f, t));
				}
				else {
					m_pText2->SetEnabled(false);
				}
			}
		}
	}
	break;

	case 4:
	{
		++m_step;
		InputManager::ChangeBindType(InputBindType::GAMEPLAY);
		m_pDefaultCamera->SetMain();
		m_finished = true;
	}
	break;
	}

	if (m_cameraMoveTimer < m_cameraMoveDuration)
	{
		m_cameraMoveTimer += Time::GetDeltaTime();
		float t = Easing::InOutQuad(m_cameraMoveTimer, m_cameraMoveDuration);
		auto transform = m_pCamera->GetTransform();
		Vector3 newPos = LerpVector3(m_prevCameraPos, m_nextCameraPos, t);
		Quaternion newRot = SlerpQuaternion(m_prevCameraRot, m_nextCameraRot, t);
		transform->SetPosition(newPos);
		transform->SetQuaternion(newRot);
	}
}
