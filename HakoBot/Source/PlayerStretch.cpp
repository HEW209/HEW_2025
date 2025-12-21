#include "PlayerStretch.h"
#include "GameFrame/Time.h"
#include "InputManager.h"

constexpr float BLOCK_SIZE = 1.0f;
constexpr int LENGTH_MAX = 4;
constexpr int INPUT_DELAY = 20;
constexpr float MAX_DELTA = 0.05f;
constexpr float PILLAR_OFFSET = 0.28f;

PlayerStretch::PlayerStretch()
	:m_targetY(0.0f)
{
}

void PlayerStretch::Update()
{
	Vector3 pos = GetTransform()->GetPosition(Space::LOCAL);

	//“ü—Í‚ðŽæ“¾
	if (InputManager::CurrentInputSystem().GetButtonHold("Up"_hash))
	{
		if(m_targetY < LENGTH_MAX)
			m_targetY = round(floor(pos.y) + BLOCK_SIZE);
	}
	else if (InputManager::CurrentInputSystem().GetButtonHold("Down"_hash))
	{
		if(0.0f < m_targetY)
			m_targetY = round(ceil(pos.y) - BLOCK_SIZE);
	}

	float diff = m_targetY - pos.y;
	if (MAX_DELTA < abs(diff))
	{
		pos.y += copysignf(MAX_DELTA, diff);
	}
	else
	{
		pos.y = m_targetY;
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
}
