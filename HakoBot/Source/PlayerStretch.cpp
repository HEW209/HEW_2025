#include "PlayerStretch.h"
#include "GameFrame/Time.h"
#include "InputManager.h"
#include "Player.h"
#include <GameFrame/ColliderSystem.h>

constexpr float BLOCK_SIZE = 1.0f;
constexpr int LENGTH_MAX = 4;
constexpr int INPUT_DELAY = 20;
constexpr float MAX_DELTA = 0.05f;
constexpr float PILLAR_OFFSET = 0.28f;

PlayerStretch::PlayerStretch()
	:m_targetY(0.0f)
	,m_targetLock(false)
{
}

void PlayerStretch::Update()
{
	Vector3 pos = GetTransform()->GetPosition(Space::LOCAL);

	// 頭上から上方向にy移動量分のレイを発射
	Vector3 rayOffset[5]=
	{
		{ 0.0f, 0.0f,  0.0f},
		{ 0.2f, 0.0f,  0.2f},
		{ 0.2f, 0.0f, -0.2f},
		{-0.2f, 0.0f,  0.2f},
		{-0.2f, 0.0f, -0.2f},
	};
	const float rayWeight = 0.5f;

	for (int i = 0; i < 5; ++i)
	{
		ColliderSystem::Ray ray;
		ray.origin = m_pCollider->GetTransform()->GetPosition(Space::WORLD) + rayOffset[i];
		ray.origin.y += m_pCollider->m_scale.y - rayWeight;
		ray.direction = Vector3::up;
		ColliderSystem::RaycastHit hit;
		if (ColliderSystem::Instance().Raycast(ray, &hit, MAX_DELTA + rayWeight))
		{
			if (0.0f < m_targetY)
			{
				m_targetY = round(ceil(pos.y) - BLOCK_SIZE);
				m_targetLock = true;
				break;
			}
		}
	}
	
	if (!m_targetLock)
	{
		if (InputManager::CurrentInputSystem().GetButtonHold("Up"_hash))
		{
			if (m_targetY < LENGTH_MAX)
				m_targetY = round(floor(pos.y) + BLOCK_SIZE);
		}
		else if (InputManager::CurrentInputSystem().GetButtonHold("Down"_hash))
		{
			if (0.0f < m_targetY)
				m_targetY = round(ceil(pos.y) - BLOCK_SIZE);
		}
	}

	float diff = m_targetY - pos.y;
	if (MAX_DELTA < abs(diff))
	{
		pos.y += copysignf(MAX_DELTA, diff);
	}
	else
	{
		pos.y = m_targetY;
		m_targetLock = false;
	}

	int pillerCount = pos.y * 5 + 2;
	for (int i = 0; i < m_pillars.size(); i++)
	{
		if (i <= pillerCount)
		{
			m_pillars[i]->SetActive(true);
		}
		else
		{
			m_pillars[i]->SetActive(false);
		}
	}

	GetTransform()->SetPosition(pos, Space::LOCAL);

	
	//当たり判定サイズの更新
	m_pCollider->m_scale.y = pos.y + PlayerDefaultSize_y;
	m_pCollider->m_positionOffset.y = m_pCollider->m_scale.y * 0.5f;
}

void PlayerStretch::LateUpdate()
{
	// 頭に付随するようにする
	Vector3 rootPos = m_pCollider->GetTransform()->GetPosition();
	Quaternion quaternion = GetTransform()->GetQuaternion();
	int max_gridStretch = 4;
	int max_pillar = max_gridStretch * 5 + 2;
	for (int i = 0; i < m_pillars.size(); ++i)
	{
		m_pillars[i]->GetTransform()->SetPosition(rootPos + Vector3(0.0f, i * 0.2f, 0.0f));
		m_pillars[i]->GetTransform()->SetQuaternion(quaternion);
	}

}
