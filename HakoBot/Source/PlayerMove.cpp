#include "PlayerMove.h"

#include "InputManager.h"
#include <GameFrame/ColliderSystem.h>

PlayerMove::PlayerMove() :
	m_moveSpeed(0.10f),
	m_jumpPower(0.5f),
	m_gravity(0.04f),
	m_velocity_y(0.0f),
	m_rotateSpeed (500.0f),
	m_IsDirLock(false),
	m_pHead(nullptr)
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
	Transform* pHead = m_pHead->GetTransform();
	GetGameObject()->GetComponent<Collider>()->OnCollisionEnter = [pHead, pObj](GameObject* other) {
		ColliderSystem::Ray ray = { pObj->GetTransform()->GetPosition() + pObj->GetTransform()->GetQuaternion() * Vector3(0.0f,0.5f,-0.35f),
									pObj->GetTransform()->GetQuaternion() * Vector3(0.0f,0.0f,-1.0f)};
		ColliderSystem::RaycastHit hit = {};
		bool isHit = false;
		float posY = pHead->GetPosition(Space::LOCAL).y;
		for (int i = 0; i < posY; ++i)	// 高さ分レイをずらして当たり判定を見る
		{
			ray.origin.y = i + 0.5f;
			if (ColliderSystem::Instance().Raycast(ray, &hit, 0.38f))
			{
				isHit = true;
				break;
			}
		}

		ray.origin.y = pHead->GetPosition().y + 0.5f;
		if (ColliderSystem::Instance().Raycast(ray, &hit, 0.38f))
		{
			isHit = true;
		}

		ray.origin.y += 0.7f;
		if (ColliderSystem::Instance().Raycast(ray, &hit, 0.38f))
		{
			isHit = true;
		}

		if (isHit)
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
	float rotatePower = moveDir.Magnitude();
	rotatePower = Math::Clamp01(rotatePower);
	moveDir = moveDir.Normalized();
	float inputMagnitude = inputVec2.Magnitude();
	move = moveDir * inputMagnitude * m_moveSpeed;
	Vector3 e = GetTransform()->GetEulerAngle();
	float currentY = GetTransform()->GetEulerAngle().y;
	if (moveDir != Vector3::zero)
	{
		float dt = 1.0f / 60.0f;
		
		// 進行方向の角度を算出
		float targetY = std::atan2(moveDir.x, moveDir.z) * (180.0f / 3.14159f);
		targetY += 180.0f;
		
		

		float maxDelta = m_rotateSpeed * dt;  // 1フレームで回せる最大角度


		float newY = MoveTowardsAngle(currentY, targetY, maxDelta * rotatePower);

		GetTransform()->SetEulerAngle(0.0f, newY, 0.0f);
	}

	// 接地判定チェック
	Vector3 pos = GetTransform()->GetPosition() + GetTransform()->GetQuaternion() * Vector3(0.0f,0.5f,-0.35f);
	ColliderSystem::Ray ray = {pos,Vector3(0.0f,-1.0f,0.0f)};
	ColliderSystem::RaycastHit hit = {};
	if (ColliderSystem::Instance().Raycast(ray, &hit, 0.7f))
	{
		// 斜面かどうか調べるための内積
		float dotSlope = ColliderSystem::Instance().Dot(Vector3(0.0f,1.0f,0.0f), hit.normal);

		if ((dotSlope > 0.99f) && inputMagnitude )
		{
			// 法線ベクトルと直行している場合、そのまま移動
			//move.y = 0.0f;
			// 重力かける
			move.y = m_velocity_y;
		}
		else
		{
			// 斜面に沿った方向を算出
			float dot = ColliderSystem::Instance().Dot(moveDir, hit.normal);
			Vector3 slopeDir = moveDir - (hit.normal * dot);
			slopeDir = slopeDir.Normalized();
			
			// 投影後の方向にスピードを掛ける
			move = slopeDir * inputMagnitude * m_moveSpeed;
		}
		m_velocity_y = 0.0f;
	}
	else
	{
		// 重力かける
		move.y = m_velocity_y;
	}

	//実際の移動
	GetTransform()->Translate(move);


	// キャタピラのUVアニメーション
	m_uvOffset.y += inputMagnitude * 0.005f;
	auto material = m_pCaterpillar->GetMaterial(0);
	material->SetParameter(&m_uvOffset, sizeof(m_uvOffset));

	// 向きロック
	if(InputManager::CurrentInputSystem().InputSystem::GetButtonHold("LockRotation"_hash))
	{
		m_IsDirLock = true;
		m_pHead->GetTransform()->SetQuaternion(m_HeadQuaternion, Space::WORLD);
	}
	else
	{
		// 解除時は胴体の向き優先
		if (m_IsDirLock)
		{
			GetTransform()->SetQuaternion(m_HeadQuaternion);
		}

		m_IsDirLock = false;
		m_pHead->GetTransform()->SetQuaternion(Quaternion::identity, Space::LOCAL);
		m_HeadQuaternion = GetTransform()->GetQuaternion();
	}
}

void PlayerMove::LateUpdate()
{
	//if (m_IsDirLock)	// ロック中
	//{
	//	m_pHead->GetTransform()->SetQuaternion(m_HeadQuaternion, Space::WORLD);
	//}
}

void PlayerMove::SetCaterpillar(MeshRenderer* renderer)
{
	m_pCaterpillar = renderer;
}
