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
	GetGameObject()->GetComponent<Collider>()->OnCollisionEnter = [pObj](GameObject* other) {
		pObj->GetComponent<SamplePlayer>()->m_velocity_y = 0.0f;
		};
}

void SamplePlayer::Update()
{
	Vector3 input;
	float rotate = 0.0f;
	Vector3 move;

	//“ü—Í‚ðŽæ“¾
	if (Input::GetKeyHold(KeyCode::A))
		input.x -= 1.0f;
	if (Input::GetKeyHold(KeyCode::D))
		input.x += 1.0f;
	if (Input::GetKeyHold(KeyCode::S))
		input.z -= 1.0f;
	if (Input::GetKeyHold(KeyCode::W))
		input.z += 1.0f;

	if (Input::GetKeyHold(KeyCode::Q))
		rotate -= 1.0f;
	if (Input::GetKeyHold(KeyCode::E))
		rotate += 1.0f;

	//ƒWƒƒƒ“ƒv
	if (Input::GetKeyDown(KeyCode::SPACE))
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
	Vector3 pos = GetTransform()->GetPosition();
	Vector3 scale = GetTransform()->GetScale();
	Quaternion qua = GetTransform()->GetQuaternion();
	Vector3 rotatevec = qua.ToEuler();
	GetTransform()->Translate(move);
	if (GetTransform()->m_position.y < 0.0f)
	{
		GetTransform()->m_position.y = 0.0f;
		m_velocity_y = 0.0f;
	}
	GetTransform()->Rotate(0.0f, rotate, 0.0f);

	if (Input::GetKeyDown(KeyCode::ENTER))
		GetTransform()->GetParent()->GetGameObject()->Destroy();
}
