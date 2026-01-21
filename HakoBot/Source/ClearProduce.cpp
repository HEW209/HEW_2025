#include "ClearProduce.h"
#include "GridField.h"
#include "GameState.h"
#include "InputManager.h"
#include "Easing.h"
#include "ResultController.h"
#include "SoundManager.h"

static const float CAMERA_ANGLE_X = 20.0f;
static float START_CAMERA_DISTANCE = 3.2f;
static float MID_CAMERA_DISTANCE = 2.0f;
static float END_CAMERA_DISTANCE = 2.7f;
constexpr float START_CAMERA_ANGLE_Y = 270.0f;
constexpr float MID_CAMERA_ANGLE_Y = -45.0f;

ClearProduce::ClearProduce():
	m_isActive(false),
	m_pCamera(nullptr),
	m_count(0),
	m_startCameraDistance(START_CAMERA_DISTANCE),
	m_midCameraDistance(MID_CAMERA_DISTANCE),
	m_endCameraDistance(END_CAMERA_DISTANCE)
{
}

void ClearProduce::Start()
{
	GridField* gridField = GameState::GetInstance()->GetGridField();
	int gridSize = gridField->GetSize().MaxElement();
	m_startCameraDistance *= gridSize;
	m_midCameraDistance *= gridSize;
	m_endCameraDistance *= gridSize;

	auto obj = SceneManager::GetActiveScene()->CreateGameObject();
	obj->GetTransform()->SetParent(GetTransform());
	obj->GetTransform()->SetEulerAngle(CAMERA_ANGLE_X, -3.0f, 0.0f, Space::LOCAL);
	m_pCamera = obj->AddComponent<Camera>();
	Camera::Config cameraConfig;
	cameraConfig.fovAngle = 30.0f;
	m_pCamera->SetConfig(cameraConfig);

	{
		auto effectObj = SceneManager::GetActiveScene()->CreateGameObject();
		effectObj->GetTransform()->SetParent(obj);
		effectObj->GetTransform()->SetPosition(8.0f, -17.0f, 10.0f);
		effectObj->GetTransform()->SetEulerAngle(0.0f, 0.0f, 25.0f);
		m_effectRenderers[0] = effectObj->AddComponent<EffectRenderer>();
		m_effectRenderers[0]->Load("Assets/Effect/Cracker/Cracker.efkefc");
	}

	{
		auto effectObj = SceneManager::GetActiveScene()->CreateGameObject();
		effectObj->GetTransform()->SetParent(obj);
		effectObj->GetTransform()->SetPosition(-8.0f, -17.0f, 10.0f);
		effectObj->GetTransform()->SetEulerAngle(0.0f, 0.0f, -25.0f);
		m_effectRenderers[1] = effectObj->AddComponent<EffectRenderer>();
		m_effectRenderers[1]->Load("Assets/Effect/Cracker/Cracker.efkefc");
	}
}



void ClearProduce::Update()
{
	GridField* gridField = GameState::GetInstance()->GetGridField();
	if (!m_isActive)
	{
		if (GameState::GetInstance()->IsClearEnter())
		{
			m_pCamera->SetMain();
			m_isActive = true;
			Player* player = GameState::GetInstance()->GetPlayer(); 
			//player->GetGameObject()->SetActive(false);
			
			// アウトライン対策
			player->GetTransform()->SetPosition(1000.0f, 0.0f, 0.0f);
		}
		return;
	}

	float cameraDistance = 0.0f;
	float cameraAngleY = 0.0f;

	++m_count;
	if (m_count < 60)
	{
		float t = static_cast<float>(m_count) / 60.0f;
		float e = Easing::OutCubic(t, 1.0f);
		float angleY = Math::Lerp(START_CAMERA_ANGLE_Y, MID_CAMERA_ANGLE_Y, e);
		float distance = Math::Lerp(m_startCameraDistance, m_midCameraDistance, e);
		Vector3 cameraLocalPos = Quaternion::Euler(CAMERA_ANGLE_X, 0.0f, 0.0f) * Vector3 { 0.0f, 0.0f, -distance } + Vector3{ 0.0f, gridField->GetSize().y * 0.5f, 0.0f };
		m_pCamera->GetTransform()->SetPosition(cameraLocalPos, Space::LOCAL);
		GetTransform()->SetEulerAngle(0.0f, angleY, 0.0f);
	}
	else if (m_count < 90)
	{
		if (m_count == 60) {
			SoundManager::PlaySE("Cracker", 1.0f, false);
			for (auto&& r : m_effectRenderers) {
				r->Play();
			}
		}

		float t = static_cast<float>(m_count - 60.0f) / 30.0f;
		float e = Easing::OutQuart(t, 1.0f);
		float distance = Math::Lerp(m_midCameraDistance, m_endCameraDistance, e);
		Vector3 cameraLocalPos = Quaternion::Euler(CAMERA_ANGLE_X, 0.0f, 0.0f) * Vector3 { 0.0f, 0.0f, -distance } + Vector3{ 0.0f, gridField->GetSize().y * 0.5f, 0.0f };
		m_pCamera->GetTransform()->SetPosition(cameraLocalPos, Space::LOCAL);
	}

	if (m_count == 100)
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->AddComponent<ResultController>();
	}
}
