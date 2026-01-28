#include "GameScene.h"
#include "StageSelectObject.h"
#include "LevelSerializer.h"
#include "SaveData.h"
#include "BlockData.h"
#include "Easing.h"

static const Vector3 g_defaultPos(0.0f, -4.0f, -1.0f);
static const Vector3 g_centerPos(0.0f, -3.0f, -1.0f);
static const Vector3 g_defaultConveyerPos(0.0f, -6.6f, 0.0f);
static const float g_blockDistance = 7.0f;
static const float g_conveyerDistance = 20.0f;
static const float g_defaultMoveSpeed = 5.0f;
static const float g_minMoveSpeed = 1.0f;
static const float g_rotateSpeed = 60.0f;
static const float g_easeScaleDuration = 0.5f;

StageSelectObject::StageSelectObject() :
	m_blocks(),
	m_easeStartFlag(false),
	m_easeStartScale(0.0f),
	m_scaleEaseTimer(0.0f)
{
}

void StageSelectObject::Start()
{
	int clearStage = SaveData::GetClearLevel();
	for (int i = 0; i < StageCount; i++)
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetTransform()->SetParent(GetTransform());
		Vector3 pos = g_defaultPos + Vector3::right * g_blockDistance * i;
		obj->GetTransform()->SetPosition(pos, Space::LOCAL);

		auto blockObj = SceneManager::GetActiveScene()->CreateGameObject();
		blockObj->GetTransform()->SetParent(obj->GetTransform());

		LevelData levelData;
		LevelSerializer::LoadLevelData("Assets/Level/Stages/Level" + std::to_string(i + 1) + ".json", levelData);
		BlockTemplateData blockData;
		if (levelData.inventoryBlockFiles.empty()) {
			LevelSerializer::LoadBlockTemplate("Assets/Level/Blocks/1masu.json", blockData);
		}
		else {
			LevelSerializer::LoadBlockTemplate("Assets/Level/Blocks/" + levelData.inventoryBlockFiles[0] + ".json", blockData);
		}
		BlockSetData blockSet;
		blockSet.blocks = blockData.blocks;
		std::string filePath = blockData.modelPath;

		Vector3 min{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
		Vector3 max{ std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
		for (auto&& blockPos : blockSet.blocks) {
			// ブロックの最小位置を記録
			min.x = std::min(min.x, (float)blockPos.x);
			min.y = std::min(min.y, (float)blockPos.y);
			min.z = std::min(min.z, (float)blockPos.z);

			// ブロックの最大位置を記録
			max.x = std::max(max.x, (float)blockPos.x);
			max.y = std::max(max.y, (float)blockPos.y);
			max.z = std::max(max.z, (float)blockPos.z);
		}
		Vector3 center = (min + max) * 0.5f;

		// ブロック（立方体）の中心から見た8つの頂点へのオフセット
		const std::vector<Vector3> cornerOffsets = {
			{ -0.5f, -0.5f, -0.5f },
			{  0.5f, -0.5f, -0.5f },
			{ -0.5f,  0.5f, -0.5f },
			{  0.5f,  0.5f, -0.5f },
			{ -0.5f, -0.5f,  0.5f },
			{  0.5f, -0.5f,  0.5f },
			{ -0.5f,  0.5f,  0.5f },
			{  0.5f,  0.5f,  0.5f }
		};

		float minY = (std::numeric_limits<float>::max)();

		const Quaternion rotation = GetTransform()->GetQuaternion();

		for (const auto& blockPos : blockSet.blocks) {
			// ブロックの中心座標
			const Vector3 center(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z));

			for (const auto& offset : cornerOffsets) {
				// ブロックの頂点座標
				Vector3 corner = center + offset;

				Vector3 rotatedCorner = rotation * corner;

				if (rotatedCorner.y < minY) {
					minY = rotatedCorner.y;
				}
			}
		}

		float groundY = (minY == (std::numeric_limits<float>::max)()) ? 0.0f : -minY;

		Vector3 offset = center * -1.0f;
		offset.y = groundY;

		blockObj->GetTransform()->SetPosition(offset, Space::LOCAL);

		if (clearStage < i)
		{
			auto mesh = blockObj->AddComponent<MeshRenderer>();
			mesh->LoadModel(filePath);
			Material* material = mesh->GetMaterial(0);
			material->SetPixelShader("Assets/Shader/OneColor_PS.cso");
			Color color(0.1f, 0.1f, 0.1f, 1.0f);
			material->SetParameter(&color, sizeof(color));
		}
		else
		{
			auto mesh = blockObj->AddComponent<OutlineMeshRenderer>();
			mesh->LoadModel(filePath);
			mesh->SetShouldDrawOutline(false);
			mesh->SetOutlineColor(Color(1.0f, 0.5f, 0.0f, 1.0f));
			mesh->SetOutlineThickness(5.0f);
			m_blockRenderers.push_back(mesh);
		}

		m_blocks[i] = obj;
	}

	for (int i = -1; i < 13; i++)
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto mesh = obj->AddComponent<MeshRenderer>();
		mesh->LoadModel("Assets/Model/Stage/FBX/Conveyors.fbx");
		obj->GetTransform()->SetParent(GetTransform());
		Vector3 pos = g_defaultConveyerPos + Vector3::right * g_conveyerDistance * i;
		obj->GetTransform()->SetPosition(pos, Space::LOCAL);
		obj->GetTransform()->SetScale(0.89f, 1.0f, 2.0f);
	}
}

void StageSelectObject::Update()
{
	Move();
	BlockMove();
	BlockScaling();
	BlockRotate();
}

void StageSelectObject::SetStageNumber(StageNumber* stageNumber)
{
	m_stageNumber = stageNumber;
}

void StageSelectObject::Move()
{
	// 目標位置計算
	Vector3 pos = GetTransform()->GetPosition();
	int selectIndex = m_stageNumber->GetSelectIndex();
	Vector3 targetPos = Vector3::zero + Vector3::left * g_blockDistance * (selectIndex - 1);

	Vector3 toTarget = targetPos - pos;
	float distance = toTarget.Magnitude();
	if (distance < 0.001f) return;

	// 移動
	Vector3 move = toTarget * g_defaultMoveSpeed;
	float mag = move.Magnitude();
	if (mag < g_minMoveSpeed)
	{
		move = move.Normalized() * g_minMoveSpeed;
	}

	if (distance < g_minMoveSpeed * Time::GetDeltaTime())
	{
		pos = targetPos; // 行き過ぎ防止
	}
	else
	{
		pos += move * Time::GetDeltaTime();
	}
	GetGameObject()->GetTransform()->SetPosition(pos);
}

void StageSelectObject::BlockMove()
{
	const float moveSpeed = 5.0f;

	// ブロックごとの移動
	int selectIndex = m_stageNumber->GetSelectIndex() - 1;
	for (int x = 0; x < StageCount; ++x)
	{
		Vector3 pos_yz = m_blocks[x]->GetTransform()->GetPosition(Space::LOCAL);
		float pos_x = pos_yz.x;
		pos_yz.x = 0.0f;

		Vector3 target;
		if (x == selectIndex)
		{
			target = g_centerPos;
			if (x < m_blockRenderers.size())
				m_blockRenderers[x]->SetShouldDrawOutline(true);
		}
		else
		{
			target = g_defaultPos;
			if (x < m_blockRenderers.size())
				m_blockRenderers[x]->SetShouldDrawOutline(false);
		}

		Vector3 toTarget = target - pos_yz;
		Vector3 move = toTarget.Normalized() * moveSpeed;

		// 移動
		if (toTarget.Magnitude() < moveSpeed * Time::GetDeltaTime())
		{
			pos_yz = target; // 行き過ぎ防止
		}
		else
		{
			pos_yz += move * Time::GetDeltaTime();
		}
		m_blocks[x]->GetTransform()->SetPosition(pos_x, pos_yz.y, pos_yz.z, Space::LOCAL);
	}
}

void StageSelectObject::BlockRotate()
{
	// ブロックごとの回転
	int selectIndex = m_stageNumber->GetSelectIndex() - 1;
	for (int x = 0; x < StageCount; ++x)
	{
		float angle_y = m_blocks[x]->GetTransform()->GetEulerAngle().y;

		if (x == selectIndex)
		{
			angle_y += g_rotateSpeed * Time::GetDeltaTime();

			if (angle_y >= 360.0f)
				angle_y -= 360.0f;
		}
		else
		{
			// 現在角度から徐々に0へ戻す
			float returnSpeed = 0.1f;
			float Lerp = angle_y + (0.0f - angle_y) * returnSpeed;	//Leap=a+(b-a)*t

			angle_y = Lerp;

			// ぶれ防止
			if (std::fabs(angle_y) < 0.01f)
				angle_y = 0.0f;
		}
		m_blocks[x]->GetTransform()->SetEulerAngle(0.0f, angle_y, 0.0f);
	}
}

void StageSelectObject::BlockScaling()
{
	int selectIndex = m_stageNumber->GetSelectIndex() - 1;

	if (m_stageNumber->IsStageStart())
	{
		if (!m_easeStartFlag)
		{
			m_easeStartScale = m_blocks[selectIndex]->GetTransform()->GetScale().x;
			m_easeStartFlag = true;
		}

		m_scaleEaseTimer += Time::GetDeltaTime();
		if (m_scaleEaseTimer > g_easeScaleDuration)
			m_scaleEaseTimer = g_easeScaleDuration;

		float scale = Easing::InBack(m_scaleEaseTimer, g_easeScaleDuration, 4.0f, 0.0f, m_easeStartScale);
		m_blocks[selectIndex]->GetTransform()->SetScale(scale, scale, scale);
	}

	// ブロックごとのスケーリング
	for (int x = 0; x < StageCount; ++x)
	{
		if (x == selectIndex && m_easeStartFlag)
			continue;

		Vector3 pos_yz = m_blocks[x]->GetTransform()->GetPosition(Space::LOCAL);
		pos_yz.x = 0.0f;

		// スケーリング
		float toCenter = (g_centerPos - pos_yz).Magnitude();
		float moveDistance = (g_centerPos - g_defaultPos).Magnitude();
		float centerRatio = 1.0f - toCenter / moveDistance;
		float scale = centerRatio * 0.5f + 1.0f;
		m_blocks[x]->GetTransform()->SetScale(scale, scale, scale);
	}
}
