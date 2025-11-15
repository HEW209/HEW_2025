#include "SamplePlayer.h"

SamplePlayer::SamplePlayer() :
	m_moveSpeed(0.2f),
	m_jumpPower(0.5f),
	m_gravity(0.05f),
	m_velocity_y(0.0f)
{
}

void SamplePlayer::Awake()
{
	GameObject* pObj = GetGameObject();
	GetGameObject()->GetComponent<Collider>()->onCollision = [pObj](GameObject* other) {
		pObj->GetTransform()->m_scale.y += 0.1;
		};
}

void SamplePlayer::Update()
{
	Vector3 input;
	Vector3 move;

	//“ü—Í‚ðŽæ“¾
	if (InputManager::GetKeyHold(Input::A))
		input.x -= 1.0f;
	if (InputManager::GetKeyHold(Input::D))
		input.x += 1.0f;
	if (InputManager::GetKeyHold(Input::S))
		input.z -= 1.0f;
	if (InputManager::GetKeyHold(Input::W))
		input.z += 1.0f;

	//ƒWƒƒƒ“ƒv
	if (InputManager::GetKeyDown(Input::SPACE))
		m_velocity_y = m_jumpPower;

	//d—Í‚ð‰ÁŽZ
	m_velocity_y -= m_gravity;

	//ˆÚ“®—Ê‚ð‹‚ß‚é
	Quaternion cameraRotation = Camera::GetMain()->GetTransform()->GetQuaternion();
	Vector3 moveDir = cameraRotation * input;
	moveDir.y = 0.0f;
	moveDir = moveDir.Normalized();
	move = moveDir * m_moveSpeed;
	move.y = m_velocity_y;

	//ŽÀÛ‚ÌˆÚ“®
	GetTransform()->TransLate(move);
	if (GetTransform()->m_position.y < 0.0f)
	{
		GetTransform()->m_position.y = 0.0f;
		m_velocity_y = 0.0f;
	}
}
