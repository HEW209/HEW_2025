#include "PlayerMove.h"

#include "InputManager.h"
#include <GameFrame/ColliderSystem.h>

PlayerMove::PlayerMove() :
	m_moveSpeed(0.10f),
	m_jumpPower(0.5f),
	m_gravity(0.04f),
	m_velocity_y(0.0f),
	m_rotateSpeed (1000.0f)
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

void PlayerMove::Start()
{
	// 上り坂の先にオブジェクトがあるとガクガクするのを防ぐための処理
	GameObject* pObj = GetGameObject();
	GetGameObject()->GetComponent<Collider>()->OnCollisionEnter = [pObj](GameObject* other) {
		ColliderSystem::Ray ray = { pObj->GetTransform()->GetPosition() + Vector3(0.0f,0.5f,0.0f), pObj->GetTransform()->GetQuaternion() * Vector3(0.0f,0.0f,-1.0f)};
		ColliderSystem::RaycastHit hit = {};
		if (ColliderSystem::Instance().Raycast(ray, &hit, 0.38f))
		{
			pObj->GetTransform()->Translate(0.0f, -pObj->GetComponent<PlayerMove>()->m_gravity, 0.0f);
			pObj->GetComponent<PlayerMove>()->m_velocity_y = 0.0f;
		}
	};
}

void PlayerMove::Update()
{
	Vector3 input;
	Vector3 move;
	float dt = (1.0f / 60.0f);
	static Vector3 euler{0.0f,0.0f ,0.0f };

	//入力を取得
	
	Vector2 inputVec2 = InputManager::CurrentInputSystem().GetAxis("Move"_hash);
	input.x = inputVec2.x;
	input.z = inputVec2.y;
		

	//
	////ジャンプ
	//if (Input::GetKeyDown(KeyCode::SPACE))
	//	m_velocity_y = m_jumpPower;
	//

	//重力を加算
	m_velocity_y -= m_gravity;

	//移動量を求める
	Quaternion cameraRotation = Camera::GetMain()->GetTransform()->GetQuaternion();
	Vector3 moveDir = cameraRotation * input;
	moveDir.y = 0.0f;
	moveDir = moveDir.Normalized();
	move = moveDir * inputVec2.Magnitude() * m_moveSpeed;
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

	// 接地判定チェック
	Vector3 pos = GetTransform()->GetPosition() + Vector3(0.0f,0.5f,0.0f);
	ColliderSystem::Ray ray = {pos,Vector3(0.0f,-1.0f,0.0f)};
	ColliderSystem::RaycastHit hit = {};
	if (ColliderSystem::Instance().Raycast(ray, &hit, 0.6f))
	{
		float dot = ColliderSystem::Instance().Dot(moveDir, hit.normal);

		// 斜面に沿った方向を算出
		Vector3 slopeDir = moveDir - (hit.normal * dot);
		slopeDir = slopeDir.Normalized();

		// 投影後の方向にスピードを掛ける
		move = slopeDir * inputVec2.Magnitude() * m_moveSpeed;

		m_velocity_y = 0.0f;
	}

	//実際の移動
	GetTransform()->Translate(move);
}
