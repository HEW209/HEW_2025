#include "StageInfo.h"
#include "LevelSerializer.h"

static const float g_windowSize = 600.0f;
static const float g_titleSize = 400.0f;
static const float g_clearShapeSize = 160.0f;
static const float g_clearShapeSpace = 10.0f;

StageInfo::StageInfo() :
	m_stageNumber(nullptr),
	m_currentStage(0)
{
}

void StageInfo::Start()
{
	auto window = GetGameObject()->AddComponent<SpriteRenderer>();
	window->LoadTexture("Assets/Textures/StageSelect/StageInfoWindow.png");
	window->SetSize(g_windowSize);
	window->SetBackGround(true);

	m_pTitle = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pTitle->LoadTexture("Assets/Textures/StageSelect/Title/title_1.png");
	m_pTitle->SetSize(g_clearShapeSize);
	m_pTitle->SetOffsetPos(0.0f, 0.95f);
	m_pTitle->SetBackGround(true);

	for (int i = 0; i < 3; i++)
	{
		auto back = GetGameObject()->AddComponent<SpriteRenderer>();
		back->LoadTexture("Assets/Textures/StageSelect/StageInfoBack.png");
		back->SetSize(g_clearShapeSize);
		m_clearShapePos[i].x = (g_clearShapeSize + g_clearShapeSpace) * (i - 1) * 0.01f;
		m_clearShapePos[i].x *= -1.0f;
		m_clearShapePos[i].y = -0.3f;
		back->SetOffsetPos(m_clearShapePos[i]);
		back->SetBackGround(true);
	}
}

void StageInfo::Update()
{
	int selectIndex = m_stageNumber->GetSelectIndex();
	if (selectIndex != m_currentStage)
	{
		CreateClearShape();
		m_pTitle->LoadTexture("Assets/Textures/StageSelect/Title/title_" + std::to_string(selectIndex) + ".png");
		m_currentStage = selectIndex;
	}
}

void StageInfo::SetStageNumber(StageNumber* stageNumber)
{
	m_stageNumber = stageNumber;
}

void StageInfo::CreateClearShape()
{
	// 古いものを削除
	for (auto& shape : m_clearShapes)
	{
		for (auto& row : shape)
		{
			for (auto& renderer : row)
			{
				if (renderer)
					renderer->Destroy();
			}
		}
	}
	m_clearShapes.clear();

	// データ読み込み
	LevelData levelData;
	LevelSerializer::LoadLevelData(m_stageNumber->GetStageFilePath(), levelData);

	// 新しく作成
	m_clearShapes.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		auto size = levelData.targetShapes[i].GetSize();
		m_clearShapes[i].resize(size.y);
		for (int y = 0; y < size.y; ++y)
		{
			m_clearShapes[i][y].resize(size.x);
			for (int x = 0; x < size.x; ++x)
			{
				Vector2 posIndex;
				posIndex.x = x - (size.x - 1) * 0.5f;
				posIndex.y = y - (size.y - 1) * 0.5f;

				float scale = g_clearShapeSize * 0.75f;
				scale /= size.x;
				Vector2 pos = m_clearShapePos[i];
				pos += posIndex * scale * 0.01f;

				auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();

				// インデックス補正
				int shape_x;
				int shape_y;
				switch (i)
				{
				case 0:
					shape_x = y;
					shape_y = x;
					break;

				case 1:
					shape_x = x;
					shape_y = y;
					break;

				case 2:
					shape_x = x;
					shape_y = y;
					break;
				}

				renderer->LoadTexture("Assets/Textures/White.png");
				if (levelData.targetShapes[i](shape_x, shape_y))
				{
					renderer->SetColor(0.1f, 0.9f, 1.0f, 1.0f);
				}
				else
				{
					renderer->SetColor(0.4f, 0.4f, 0.4f, 1.0f);
				}
				renderer->SetSize(scale);
				renderer->SetOffsetPos(pos);
				renderer->SetBackGround(true);

				m_clearShapes[i][y][x] = renderer;
			}
		}
	}
}