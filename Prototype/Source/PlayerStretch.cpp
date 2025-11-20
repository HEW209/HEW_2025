#include "PlayerStretch.h"

constexpr float BLOCK_SIZE = 1.0f;
constexpr int LENGTH_MAX = 4;
constexpr int INPUT_DELAY = 20;
constexpr float MAX_DELTA = 0.05f;
constexpr float PILLAR_OFFSET = 0.28f;

PlayerStretch::PlayerStretch()
	:m_targetY(0.0f)
	,m_pillar(nullptr)
{
}

void PlayerStretch::Update()
{
	Vector3 pos = GetTransform()->GetPosition(Space::LOCAL);
	Vector3 scale = m_pillar->GetTransform()->GetScale();
	Vector3 pillarPos = m_pillar->GetTransform()->GetPosition(Space::LOCAL);

	//“ü—Í‚ðŽæ“¾
	if (InputManager::GetKeyHold(Input::SPACE))
	{
		if(m_targetY < LENGTH_MAX)
			m_targetY = round(floor(pos.y) + BLOCK_SIZE);
	}
	else if (InputManager::GetKeyHold(Input::CTRL))
	{
		if(0.0f < m_targetY)
			m_targetY = round(ceil(pos.y) - BLOCK_SIZE);
	}

	float diff = m_targetY - pos.y;
	if (MAX_DELTA < abs(diff))
	{
		pos.y += copysignf(MAX_DELTA, diff);
		scale.y += copysignf(MAX_DELTA, diff);
		pillarPos.y -= PILLAR_OFFSET * copysignf(MAX_DELTA, diff);
	}
	else
	{
		pos.y = m_targetY;
		scale.y = m_targetY + 1.0f;
		pillarPos.y = PILLAR_OFFSET * -m_targetY;
	}

	GetTransform()->SetPosition(pos);
	m_pillar->GetTransform()->SetScale(scale);
	m_pillar->GetTransform()->SetPosition(pillarPos);
}
