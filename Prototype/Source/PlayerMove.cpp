#include "PlayerMove.h"

PlayerMove::PlayerMove() :
	m_moveSpeed(0.10f),
	m_jumpPower(0.5f),
	m_gravity(0.05f),
	m_velocity_y(0.0f),
	m_rotateSpeed (500.0f)

{
}

float DeltaAngle(float current, float target)
{
	float delta = fmodf(target - current, 360.0f);
	if (delta > 180.0f)
		delta -= 360.0f;
	else if (delta < -180.0f)
		delta += 360.0f;
	return delta;
}

// Mathf::MoveTowardsAngle と同等の関数
float MoveTowardsAngle(float current, float target, float maxDelta)
{
	float delta = DeltaAngle(current, target);

	// すでに maxDelta 以内なら、いきなり目標角度にしてしまう
	if (std::fabs(delta) <= maxDelta)
		return target;

	// delta の符号方向に、maxDelta だけ動かす
	if (delta > 0.0f)
	{
		return current + maxDelta;
	}
	else
	{
		return current - maxDelta;
	}
	return current + (delta > 0.0f ? maxDelta : -maxDelta);
}

void PlayerMove::Update()
{
	Vector3 input;
	Vector3 move;
	float dt = (1.0f / 60.0f);
	static Vector3 euler{0.0f,0.0f ,0.0f };

	//入力を取得
	if (Input::GetKeyHold(KeyCode::A))
	{
		input.x -= 1.0f;
	}
	if (Input::GetKeyHold(KeyCode::D))
	{
		input.x += 1.0f;
	}
	if (Input::GetKeyHold(KeyCode::S))
	{
		input.z -= 1.0f;
	}
	if (Input::GetKeyHold(KeyCode::W))
	{
		input.z += 1.0f;
	}
		

	/*
	//ジャンプ
	if (Input::GetKeyDown(KeyCode::SPACE))
		m_velocity_y = m_jumpPower;
	*/

	//重力を加算
	m_velocity_y -= m_gravity;

	//移動量を求める
	Quaternion cameraRotation = Camera::GetMain()->GetTransform()->GetQuaternion();
	Vector3 moveDir = cameraRotation * input;
	moveDir.y = 0.0f;
	moveDir = moveDir.Normalized();
	move = moveDir * m_moveSpeed;
	move.y = m_velocity_y;
	Vector3 e = GetTransform()->GetEulerAngle();
	float currentY = GetTransform()->GetEulerAngle().y;
	if (moveDir != Vector3::zero)
	{
		float dt = 1.0f / 60.0f;
		
		// 進行方向の角度を算出
		float targetY = std::atan2(moveDir.x, moveDir.z) * (180.0f / 3.14159f);
		targetY += 180.0f;
		
		

		float maxDelta = m_rotateSpeed * dt;  // 1フレームで回せる最大角度


		float newY = MoveTowardsAngle(currentY, targetY, maxDelta);

		GetTransform()->SetEulerAngle(0.0f, newY, 0.0f);
	}
	

	//実際の移動
	GetTransform()->Translate(move);
	if (GetTransform()->m_position.y < 0.0f)
	{
		GetTransform()->m_position.y = 0.0f;
		m_velocity_y = 0.0f;
	}


}
