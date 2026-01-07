#include "GameScene.h"
#include "StageSelectObject.h"
#include "LevelSerializer.h"
#include "SaveData.h"

static const Vector3 g_defaultPos(0.0f, -3.5f, -1.0f);
static const Vector3 g_centerPos(0.0f, -2.5f, -1.0f);
static const Vector3 g_defaultConveyerPos(0.0f, -6.6f, 0.0f);
static const float g_blockDistance = 7.0f;
static const float g_conveyerDistance = 20.0f;
static const float g_defaultMoveSpeed = 5.0f;
static const float g_minMoveSpeed = 1.0f;
static const float g_rotateSpeed = 60.0f;

static const char* g_blockNames[StageCount] =
{
	"1masu",
	"Lji1",
	"tate2masu",
	"tate3masu",
	"yoko2masu",
	"yoko3masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu",
	"1masu"
};

void StageSelectObject::Start()
{
	int clearStage = SaveData::GetClearLevel();
	for (int i = 0; i < StageCount; i++)
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetTransform()->SetParent(GetTransform());
		Vector3 pos = g_defaultPos + Vector3::right * g_blockDistance * i;
		obj->GetTransform()->SetPosition(pos, Space::LOCAL);

		std::string filePath = "Assets/Model/Blocks/FBX/";
		filePath += g_blockNames[i];
		filePath += ".fbx";

		if (clearStage < i)
		{
			auto mesh = obj->AddComponent<MeshRenderer>();
			mesh->LoadModel(filePath);
			Material* material = mesh->GetMaterial(0);
			material->SetPixelShader("Assets/Shader/OneColor_PS.cso");
			Color color(0.1f, 0.1f, 0.1f, 1.0f);
			material->SetParameter(&color, sizeof(color));
		}
		else
		{
			auto mesh = obj->AddComponent<OutlineMeshRenderer>();
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
	BlockRotate();

#ifdef _DEBUG
	float deltaTime = Time::GetDeltaTime();
	int fps = 1.0f / deltaTime;

	ImGui::Begin("Guide");
	ImGui::Text("FPS : %3d", fps);
	ImGui::End();
#endif // DEBUG
}

void StageSelectObject::SetStageNumber(StageNumber* stageNumber)
{
	m_stageNumber = stageNumber;
}

void StageSelectObject::Move()
{
	// ñ⁄ïWà íuåvéZ
	Vector3 pos = GetTransform()->GetPosition();
	int selectIndex = m_stageNumber->GetSelectIndex();
	Vector3 targetPos = Vector3::zero + Vector3::left * g_blockDistance * (selectIndex - 1);

	Vector3 toTarget = targetPos - pos;
	float distance = toTarget.Magnitude();
	if (distance < 0.001f) return;

	// à⁄ìÆ
	Vector3 move = toTarget * g_defaultMoveSpeed;
	float mag = move.Magnitude();
	if (mag < g_minMoveSpeed)
	{
		move = move.Normalized() * g_minMoveSpeed;
	}

	if (distance < g_minMoveSpeed * Time::GetDeltaTime())
	{
		pos = targetPos; // çsÇ´âﬂÇ¨ñhé~
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

	// ÉuÉçÉbÉNÇ≤Ç∆ÇÃà⁄ìÆ
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

		// à⁄ìÆ
		if (toTarget.Magnitude() < moveSpeed * Time::GetDeltaTime())
		{
			pos_yz = target; // çsÇ´âﬂÇ¨ñhé~
		}
		else
		{
			pos_yz += move * Time::GetDeltaTime();
		}
		m_blocks[x]->GetTransform()->SetPosition(pos_x, pos_yz.y, pos_yz.z, Space::LOCAL);

		// ÉXÉPÅ[ÉäÉìÉO
		float toCenter = (g_centerPos - pos_yz).Magnitude();
		float moveDistance = (g_centerPos - g_defaultPos).Magnitude();
		float centerRatio = 1.0f - toCenter / moveDistance;
		float scale = centerRatio * 0.5f + 1.0f;
		m_blocks[x]->GetTransform()->SetScale(scale, scale, scale);
	}
}

void StageSelectObject::BlockRotate()
{
	// ÉuÉçÉbÉNÇ≤Ç∆ÇÃâÒì]
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
			// åªç›äpìxÇ©ÇÁèôÅXÇ…0Ç÷ñﬂÇ∑
			float returnSpeed = 0.1f;
			float Lerp = angle_y + (0.0f - angle_y) * returnSpeed;	//Leap=a+(b-a)*t

			angle_y = Lerp;

			// Ç‘ÇÍñhé~
			if (std::fabs(angle_y) < 0.01f)
				angle_y = 0.0f;
		}
		m_blocks[x]->GetTransform()->SetEulerAngle(0.0f, angle_y, 0.0f);
	}
}
