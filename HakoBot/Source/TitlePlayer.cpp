#include "TitlePlayer.h"

static const Vector3 g_startPos(-15.0f, 0.0f, 0.0f);
static const Vector3 g_endPos(15.0f, 0.0f, 0.0f);
static const Vector3 g_blockPos(0.0f, 2.3f, -0.2f);

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
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/1masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/Lji1.fbx", {0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/tate2masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/tate3masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/yoko2masu.fbx", {0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/yoko3masu.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/teto.fbx", {-0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/ebi.fbx", {0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/Barrel.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/Tree.fbx", {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/FirePlace.fbx", {-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/Kotatu.fbx", {-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f, 0.0f} });
	m_blocks.push_back({ "Assets/Model/Blocks/FBX/Shachihoko.fbx", {-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });

	auto blockObj = SceneManager::GetActiveScene()->CreateGameObject();
	blockObj->GetTransform()->SetParent(GetTransform());
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
	int randIndex = rand() % m_blocks.size();
	m_block->LoadModel(m_blocks[randIndex].filePath);
	m_block->GetTransform()->SetPosition(g_blockPos + m_blocks[randIndex].offset, Space::LOCAL);
	m_block->GetTransform()->SetEulerAngle(m_blocks[randIndex].angle, Space::LOCAL);
}
