#include "Tutorial.h"
#include "VecUtil.h"
#include "Easing.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "Vec.h"

ITutorial::~ITutorial()
{
}


Tutorial1::Tutorial1()
	: m_finished(false)
	, m_sePlayed(false)
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
		for (auto& played : m_sePlayed)
		{
			played = false;
		}
		++m_step;
	}
	break;

	case 1:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			m_pLine->SetEnabled(true);

			if (!m_sePlayed[0])
			{
				SoundManager::PlaySE("Announcement", 0.9f, false);
				m_sePlayed[0] = true;
			}

			if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
				SoundManager::PlaySE("Tutorial_Decision", 1.0f, false);
				++m_step;
				auto transform = m_pCamera->GetTransform();
				m_prevCameraPos = transform->GetPosition();
				m_prevCameraRot = transform->GetQuaternion();
				m_nextCameraPos = Vector3(-5.0f, 4.0f, -5.0f);
				m_nextCameraRot = Quaternion::Euler(30.0f, 300.0f, 0.0f);
				m_cameraMoveDuration = 2.0f;
				m_cameraMoveTimer = 0.0f;
				m_pLine->SetEnabled(false);
				SoundManager::PlaySE("TextBoxOut", 1.0f, false);
			}
		}
		else {
			{
				float rate = Math::Clamp01((m_cameraMoveTimer - 1.5f) * 2.0f);
				float t = Easing::OutBack(rate, 1.0f, 1.7f);
				m_pText1->SetOffsetPos(0.0f, std::lerp(-7.0f, 0.0f, t));
				if (!m_pText1->IsEnabled()) {
					m_pText1->SetEnabled(true);
				}
				if (t > 0.0f && !m_sePlayed[1]) {
					SoundManager::PlaySE("TextBoxIn", 1.0f, false);
					m_sePlayed[1] = true;
				}
			}
		}
	}
	break;

	case 2:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
				SoundManager::PlaySE("Tutorial_Decision", 1.0f, false);
				++m_step;
				auto transform = m_pCamera->GetTransform();
				m_prevCameraPos = transform->GetPosition();
				m_prevCameraRot = transform->GetQuaternion();
				m_nextCameraPos = m_pDefaultCamera->GetTransform()->GetPosition();
				m_nextCameraRot = m_pDefaultCamera->GetTransform()->GetQuaternion();
				m_cameraMoveDuration = 2.0f;
				m_cameraMoveTimer = 0.0f;
				SoundManager::PlaySE("TextBoxOut", 1.0f, false);
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
				if (!m_pText2->IsEnabled()) {
					m_pText2->SetEnabled(true);
				}
				if (t > 0.0f && !m_sePlayed[2]) {
					SoundManager::PlaySE("TextBoxIn", 1.0f, false);
					m_sePlayed[2] = true;
				}
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

Tutorial2::Tutorial2()
	: m_finished(false)
	, m_sePlayed(false)
	, m_cameraMoveDuration(0.0f)
	, m_cameraMoveTimer(0.0f)
	, m_step(0)
	, m_arrowMoveTimer(0.0f)
	, m_arrowAlpha(1.0f)
{
}

void Tutorial2::Awake()
{
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetTransform()->SetParent(GetTransform());
		m_pCamera = obj->AddComponent<Camera>();
		Camera::Config cameraConfig;
		cameraConfig.fovAngle = 30.0f;
		m_pCamera->SetConfig(cameraConfig);
	}
	m_pText = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pText->LoadTexture("Assets/Textures/Tutorial/tutorial2_text1.png", false);
	m_pText->SetSize(1280.0f);
	m_pText->SetUI(true);
	m_pText->SetEnabled(false);

	for (int i = 0; i < 2; ++i)
	{
		auto arrowObj = SceneManager::GetActiveScene()->CreateGameObject();
		arrowObj->GetTransform()->SetPosition(-10.5f, 0.8f, -5.5f);
		
		auto arrow = arrowObj->AddComponent<MeshRenderer>();
		if (i == 1)
		{
			arrow->LoadModel("Assets/Model/TutorialUI/fbx/L_Arrow.fbx");
		}
		else
		{
			arrow->LoadModel("Assets/Model/TutorialUI/fbx/R_Arrow.fbx");
		}
		m_rotateArrow[i] = arrow;
	}
}

void Tutorial2::LateUpdate()
{
	m_arrowMoveTimer += Time::GetDeltaTime();
	float rad = m_arrowMoveTimer * Math::TAU / 2.0f;
	float offset = std::fabsf(std::sinf(rad)) * 5.0f;

	m_rotateArrow[0]->GetTransform()->SetEulerAngle(0.0f, -105.0f - offset, 0.0f);
	m_rotateArrow[1]->GetTransform()->SetEulerAngle(0.0f, -30.0f + offset, 0.0f);

	if (m_step >= 3)
	{
		if (m_arrowAlpha > 0.0f)
		{
			m_arrowAlpha -= Time::GetDeltaTime() * 2.0f;
			if (m_arrowAlpha < 0.0f)
				m_arrowAlpha = 0.0f;

			for (int i = 0; i < 2; ++i)
			{
				m_rotateArrow[i]->SetTransparent(true);
				auto material = m_rotateArrow[i]->GetMaterial(0);
				material->SetBlendState(BlendState::ALPHA);
				material->SetDepthStencilState(DepthStencilState::READ_ONLY);
				material->SetPixelShader("Assets/Shader/Transparent_PS.cso");
				m_rotateArrow[i]->GetMaterial(0)->SetParameter(&m_arrowAlpha, sizeof(m_arrowAlpha));
			}
		}
	}

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
		m_nextCameraPos = Vector3(-4.0f, 3.5f, -8.0f);
		m_nextCameraRot = Quaternion::Euler(25.0f, 300.0f, 0.0f);
		m_cameraMoveDuration = 2.0f;
		m_cameraMoveTimer = 0.0f;
		m_pCamera->SetMain();

		InputManager::ChangeBindType(InputBindType::UI);
		for (auto& played : m_sePlayed)
		{
			played = false;
		}
		++m_step;
	}
	break;

	case 1:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
				SoundManager::PlaySE("Tutorial_Decision", 1.0f, false);
				++m_step;
			}
		}
		else {
			{
				float rate = Math::Clamp01((m_cameraMoveTimer - 1.5f) * 2.0f);
				float t = Easing::OutBack(rate, 1.0f, 1.7f);
				m_pText->SetOffsetPos(0.0f, std::lerp(-7.0f, 0.0f, t));
				if (!m_pText->IsEnabled()) {
					m_pText->SetEnabled(true);
				}
				if (t > 0.0f && !m_sePlayed[1]) {
					SoundManager::PlaySE("TextBoxIn", 1.0f, false);
					m_sePlayed[1] = true;
				}
			}
		}
	}
	break;

	case 2:
	{
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
			SoundManager::PlaySE("Tutorial_Decision", 1.0f, false);
			++m_step;
			auto transform = m_pCamera->GetTransform();
			m_prevCameraPos = transform->GetPosition();
			m_prevCameraRot = transform->GetQuaternion();
			m_nextCameraPos = m_pDefaultCamera->GetTransform()->GetPosition();
			m_nextCameraRot = m_pDefaultCamera->GetTransform()->GetQuaternion();
			m_cameraMoveDuration = 2.0f;
			m_cameraMoveTimer = 0.0f;
			SoundManager::PlaySE("TextBoxOut", 1.0f, false);
		}
		else {
			m_pText->LoadTexture("Assets/Textures/Tutorial/tutorial2_text2.png", false);
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
					m_pText->SetOffsetPos(0.0f, std::lerp(0.0f, 7.0f, t));
				}
				else {
					m_pText->SetEnabled(false);
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

Tutorial3::Tutorial3()
	: m_finished(false)
	, m_sePlayed(false)
	, m_cameraMoveDuration(0.0f)
	, m_cameraMoveTimer(0.0f)
	, m_step(0)
	, m_arrowMoveTimer(0.0f)
	, m_arrowAlpha(1.0f)
	, m_lightAngleY(0.0f)
{
}

void Tutorial3::Awake()
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
	m_pLine->LoadTexture("Assets/Textures/Tutorial/tutorial3_line.png", false);
	m_pLine->SetSize(1280.0f);
	m_pLine->SetUI(true);
	m_pLine->SetEnabled(false);
	m_pText1 = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pText1->LoadTexture("Assets/Textures/Tutorial/tutorial3_text1.png", false);
	m_pText1->SetSize(1280.0f);
	m_pText1->SetUI(true);
	m_pText1->SetEnabled(false);
	m_pText2 = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pText2->LoadTexture("Assets/Textures/Tutorial/tutorial3_text2.png", false);
	m_pText2->SetSize(1280.0f);
	m_pText2->SetUI(true);
	m_pText2->SetEnabled(false);

	m_lightAngleY = 90.0f;
	DirectionalLight::GetMain()->GetTransform()->SetEulerAngle(50.0f, m_lightAngleY, 0.0f);
	for (int i = 0; i < 2; ++i)
	{
		auto arrowObj = SceneManager::GetActiveScene()->CreateGameObject();
		arrowObj->GetTransform()->SetScale(0.6f, 0.6f, 0.6f);

		if (i == 0)
		{
			arrowObj->GetTransform()->SetPosition(-1.5f, 1.5f, -10.5f);
			arrowObj->GetTransform()->SetEulerAngle(0.0f, -60.0f, 0.0f);
		}
		else
		{
			arrowObj->GetTransform()->SetPosition(-1.5f, 0.5f, -10.5f);
			arrowObj->GetTransform()->SetEulerAngle(0.0f, -60.0f, 180.0f);
		}

		auto arrow = arrowObj->AddComponent<MeshRenderer>();
		arrow->LoadModel("Assets/Model/TutorialUI/fbx/Arrow.fbx");
		m_arrow[i] = arrow;
	}
}

void Tutorial3::LateUpdate()
{
	m_arrowMoveTimer += Time::GetDeltaTime();
	float rad = m_arrowMoveTimer * Math::TAU / 2.0f;
	float offset = std::fabsf(std::sinf(rad)) * 0.1f;

	m_arrow[0]->GetTransform()->SetPosition(-1.5f, 1.5f + offset, -10.5f);
	m_arrow[1]->GetTransform()->SetPosition(-1.5f, 0.5f - offset, -10.5f);

	if (m_step >= 3)
	{
		if (m_arrowAlpha > 0.0f)
		{
			m_arrowAlpha -= Time::GetDeltaTime() * 2.0f;
			if (m_arrowAlpha < 0.0f)
				m_arrowAlpha = 0.0f;

			for (int i = 0; i < 2; ++i)
			{
				m_arrow[i]->SetTransparent(true);
				auto material = m_arrow[i]->GetMaterial(0);
				material->SetBlendState(BlendState::ALPHA);
				material->SetDepthStencilState(DepthStencilState::READ_ONLY);
				material->SetPixelShader("Assets/Shader/Transparent_PS.cso");
				m_arrow[i]->GetMaterial(0)->SetParameter(&m_arrowAlpha, sizeof(m_arrowAlpha));
			}
		}

		m_lightAngleY -= Time::GetDeltaTime() * 360.0f;
		if (m_lightAngleY < -15.0f)
			m_lightAngleY = -15.0f;
		DirectionalLight::GetMain()->GetTransform()->SetEulerAngle(50.0f, m_lightAngleY, 0.0f);
	}

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
		for (auto& played : m_sePlayed)
		{
			played = false;
		}
		++m_step;
	}
	break;

	case 1:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			m_pLine->SetEnabled(true);

			if (!m_sePlayed[0])
			{
				SoundManager::PlaySE("Announcement", 0.9f, false);
				m_sePlayed[0] = true;
			}

			if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
				SoundManager::PlaySE("Tutorial_Decision", 1.0f, false);
				++m_step;
				auto transform = m_pCamera->GetTransform();
				m_prevCameraPos = transform->GetPosition();
				m_prevCameraRot = transform->GetQuaternion();
				m_nextCameraPos = Vector3(-4.3f, 2.5f, -5.5f);
				m_nextCameraRot = Quaternion::Euler(10.0f, 151.0f, 0.0f);
				m_cameraMoveDuration = 2.0f;
				m_cameraMoveTimer = 0.0f;
				m_pLine->SetEnabled(false);
				SoundManager::PlaySE("TextBoxOut", 1.0f, false);
			}
		}
		else {
			{
				float rate = Math::Clamp01((m_cameraMoveTimer - 1.5f) * 2.0f);
				float t = Easing::OutBack(rate, 1.0f, 1.7f);
				m_pText1->SetOffsetPos(0.0f, std::lerp(-7.0f, 0.0f, t));
				if (!m_pText1->IsEnabled()) {
					m_pText1->SetEnabled(true);
				}
				if (t > 0.0f && !m_sePlayed[1]) {
					SoundManager::PlaySE("TextBoxIn", 1.0f, false);
					m_sePlayed[1] = true;
				}
			}
		}
	}
	break;

	case 2:
	{
		if (m_cameraMoveTimer >= m_cameraMoveDuration) {
			if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash)) {
				SoundManager::PlaySE("Tutorial_Decision", 1.0f, false);
				++m_step;
				auto transform = m_pCamera->GetTransform();
				m_prevCameraPos = transform->GetPosition();
				m_prevCameraRot = transform->GetQuaternion();
				m_nextCameraPos = m_pDefaultCamera->GetTransform()->GetPosition();
				m_nextCameraRot = m_pDefaultCamera->GetTransform()->GetQuaternion();
				m_cameraMoveDuration = 2.0f;
				m_cameraMoveTimer = 0.0f;
				SoundManager::PlaySE("TextBoxOut", 1.0f, false);
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
				if (!m_pText2->IsEnabled()) {
					m_pText2->SetEnabled(true);
				}
				if (t > 0.0f && !m_sePlayed[2]) {
					SoundManager::PlaySE("TextBoxIn", 1.0f, false);
					m_sePlayed[2] = true;
				}
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
		switch (m_step)
		{
		case 3:
		{
			float t = Easing::InOutQuad(m_cameraMoveTimer, m_cameraMoveDuration);
			float ePos = Easing::InQuad(t, 1.0f);
			auto transform = m_pCamera->GetTransform();
			Vector3 newPos = LerpVector3(m_prevCameraPos, m_nextCameraPos, ePos);
			newPos.x += std::sin(t * Math::PI) * 5.0f;
			Quaternion newRot = SlerpQuaternion(m_prevCameraRot, m_nextCameraRot, t);
			transform->SetPosition(newPos);
			transform->SetQuaternion(newRot);
		}
		break;

		default:
		{
			float t = Easing::InOutQuad(m_cameraMoveTimer, m_cameraMoveDuration);
			auto transform = m_pCamera->GetTransform();
			Vector3 newPos = LerpVector3(m_prevCameraPos, m_nextCameraPos, t);
			Quaternion newRot = SlerpQuaternion(m_prevCameraRot, m_nextCameraRot, t);
			transform->SetPosition(newPos);
			transform->SetQuaternion(newRot);
		}
		break;
		}
	}
}
