#include "TitlePlayer.h"
#include "LevelSerializer.h"
#include "SaveData.h"
#include "BlockData.h"

static const Vector3 g_startPos(-15.0f, 0.0f, 0.0f);
static const Vector3 g_endPos(15.0f, 0.0f, 0.0f);
static const Vector3 g_blockPos(0.0f, 1.8f, -0.2f);

TitlePlayer::TitlePlayer():
	m_isStart(true),
	m_block(nullptr),
	m_caterpillar(nullptr)
{
}

void TitlePlayer::Start()
{
	auto upper = GetGameObject()->AddComponent<MeshRenderer>();
	upper->LoadModel("Assets/Model/Player/fbx/jyoubu.fbx");
	upper->SetShouldDrawShadow(true);
	upper->GetMaterial(0)->SetVertexShader("Assets/Shader/Anime_VS.cso");
	auto animeNo = upper->LoadAnimation("Assets/Model/Player/fbx/motiage.fbx");
	upper->PlayAnime(animeNo, true);

	// 目
	auto eyeObj = SceneManager::GetActiveScene()->CreateGameObject();
	eyeObj->GetTransform()->SetParent(GetTransform());
	eyeObj->GetTransform()->SetPosition(0.0f, 0.0f, -0.3f, Space::LOCAL);
	auto eye = eyeObj->AddComponent<MeshRenderer>();
	eye->LoadModel("Assets/Model/Player/fbx/eye.fbx");
	auto eyeAnimeNo = eye->LoadAnimation("Assets/Model/Player/fbx/eye.fbx");
	eye->GetMaterial(0)->SetVertexShader("Assets/Shader/Anime_VS.cso");
	eye->GetMaterial(0)->SetPixelShader("Assets/Shader/OneColor_PS.cso");
	Color eyeColor(0.0f, 0.5f, 0.8f, 1.0f);
	eye->GetMaterial(0)->SetParameter(&eyeColor, sizeof(eyeColor));
	eye->PlayAnime(eyeAnimeNo, true);

	auto pillar = GetGameObject()->AddComponent<MeshRenderer>();
	pillar->LoadModel("Assets/Model/Player/fbx/sityu.fbx");
	pillar->SetShouldDrawShadow(true);

	auto lower = GetGameObject()->AddComponent<MeshRenderer>();
	lower->LoadModel("Assets/Model/Player/fbx/kabu.fbx");
	lower->SetShouldDrawShadow(true);

	m_caterpillar = GetGameObject()->AddComponent<MeshRenderer>();
	m_caterpillar->LoadModel("Assets/Model/Player/fbx/kyatapira.fbx");
	m_caterpillar->GetMaterial(0)->SetPixelShader("Assets/Shader/UVScroll_PS.cso");
	m_caterpillar->SetShouldDrawShadow(true);

	GetTransform()->SetPosition(g_startPos);
	GetTransform()->SetEulerAngle(0.0f, -90.0f, 0.0f);

	// ブロック登録
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/1masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/Lji1.fbx", {0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/tate2masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/tate3masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/yoko2masu.fbx", {0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/yoko3masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/teto.fbx", {-0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/ebi.fbx", {0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/Barrel.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/Tree.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/FirePlace.fbx", {-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/Kotatu.fbx", {-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f, 0.0f} });
	//m_blocks.push_back({ "Assets/Model/Blocks/FBX/Shachihoko.fbx", {-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });

	//auto blockObj = SceneManager::GetActiveScene()->CreateGameObject();
	//blockObj->GetTransform()->SetParent(GetTransform());
	//blockObj->GetTransform()->SetPosition(g_blockPos, Space::LOCAL);
	//m_block = blockObj->AddComponent<MeshRenderer>();
	//m_block->SetShouldDrawShadow(true);

	m_pBlockHolder = SceneManager::GetActiveScene()->CreateGameObject();
	m_pBlockHolder->GetTransform()->SetParent(GetTransform());
	m_pBlockHolder->GetTransform()->SetPosition(g_blockPos, Space::LOCAL);

	auto blockObj = SceneManager::GetActiveScene()->CreateGameObject();
	blockObj->GetTransform()->SetParent(m_pBlockHolder->GetTransform());
	blockObj->GetTransform()->SetPosition(g_blockPos, Space::LOCAL);
	m_block = blockObj->AddComponent<MeshRenderer>();
	m_block->SetShouldDrawShadow(true);

	SetRandomBlock();
}

void TitlePlayer::Update()
{
	// 移動
	Vector3 pos = GetTransform()->GetPosition();
	bool goal = false;
	float moveSpeed = 5.0f;

	if (m_isStart)
	{
		pos.x = Math::MoveTowards(pos.x, g_endPos.x, moveSpeed * Time::GetDeltaTime());
		if (pos.x == g_endPos.x)
			goal = true;
	}
	else
	{
		pos.x = Math::MoveTowards(pos.x, g_startPos.x, moveSpeed * Time::GetDeltaTime());
		if (pos.x == g_startPos.x)
			goal = true;
	}
	GetTransform()->SetPosition(pos);

	if (goal)
	{
		m_isStart = !m_isStart;
		GetTransform()->Rotate(0.0f, 180.0f, 0.0f);

		SetRandomBlock();
	}


	// キャタピラのUVアニメーション
	m_uvOffset.y +=  0.005f;
	auto material = m_caterpillar->GetMaterial(0);
	material->SetParameter(&m_uvOffset, sizeof(m_uvOffset));
}

void TitlePlayer::SetRandomBlock()
{
	//int randIndex = rand() % m_blocks.size();
	//m_block->LoadModel(m_blocks[randIndex].filePath);
	//m_block->GetTransform()->SetPosition(g_blockPos + m_blocks[randIndex].offset, Space::LOCAL);
	//m_block->GetTransform()->SetEulerAngle(m_blocks[randIndex].angle, Space::LOCAL);

	SaveData::Load();
	int clearStage = SaveData::GetClearLevel();

	int randIndex = rand() % std::max(clearStage, 5) + 1;
	LevelData levelData;
	LevelSerializer::LoadLevelData("Assets/Level/Stages/Level" + std::to_string(randIndex) + ".json", levelData);
	BlockTemplateData blockData;
	if (levelData.inventoryBlockFiles.empty()) {
		LevelSerializer::LoadBlockTemplate("Assets/Level/Blocks/1masu.json", blockData);
	}
	else {
		LevelSerializer::LoadBlockTemplate("Assets/Level/Blocks/" + levelData.inventoryBlockFiles[0] + ".json", blockData);
	}
	BlockSetData blockSet;
	blockSet.blocks = blockData.blocks;

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

	m_block->LoadModel(blockData.modelPath);
	m_block->GetTransform()->SetPosition(offset, Space::LOCAL);
}
