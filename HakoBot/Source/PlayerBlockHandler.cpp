#include "PlayerBlockHandler.h"

#include "GameState.h"
#include "InputManager.h"
#include "InputSystem.h"
#include "SoundMaster.h"
#include "SoundManager.h"
#include "VecUtil.h"


static const float g_blendTime = 0.1f;

Vec2 CalcSpacedRectPos(const Vec2& rectSize, float distance, const Vec2& direction);


PlayerBlockHandler::PlayerBlockHandler():
	m_okuAnime(MeshGroup::ANIME_NONE),
	m_motuAnime(MeshGroup::ANIME_NONE)
{
}

void PlayerBlockHandler::Awake()
{
	m_pBlockHolder = SceneManager::GetActiveScene()->CreateGameObject();

	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetComponent<Transform>()->SetParent(m_pBlockHolder->GetTransform());
		m_pBlockObject = obj->AddComponent<BlockObject>();
		m_pBlockObject->SetUseCollider(false);
	}

	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		m_pPlaceCursor = obj->AddComponent<PlaceCursor>();
		obj->SetActive(false);
	}

	m_pRenderer = GetGameObject()->GetComponent<MeshRenderer>();
	m_okuAnime = m_pRenderer->LoadAnimation("Assets/Model/Player/fbx/idle.fbx");
	m_motuAnime = m_pRenderer->LoadAnimation("Assets/Model/Player/fbx/motiage.fbx");
	auto materials = m_pRenderer->GetMaterials();
	for (auto& material : *materials)
	{
		material.SetVertexShader("Assets/Shader/Anime_VS.cso");
	}

	m_pRenderer->PlayAnime(m_okuAnime, true);
}

void PlayerBlockHandler::LateUpdate()
{
	auto blockTransform = m_pBlockObject->GetTransform();
	auto playerTransform = GetTransform();
	auto holderTransform = m_pBlockHolder->GetTransform();

	const bool isHoldingBlock = !m_pBlockObject->GetBlockSet().blocks.empty();


	Vector3 blockOffset{ 0.0f, 1.5f, 0.0f };
	//地面に設置させるために計算
	blockOffset += m_pBlockObject->GetGroundOffset();

	holderTransform->SetPosition(playerTransform->GetPosition() + blockOffset);


	if (InputManager::CurrentInputSystem().GetButtonDown("RotateBlockRight"_hash)) {
		if (isHoldingBlock)
		{
			SoundManager::PlaySE("BlockRotate", 1.0f, false);
		}
		holderTransform->Rotate(0.0f, -90.0f, 0.0f);

	}
	if (InputManager::CurrentInputSystem().GetButtonDown("RotateBlockLeft"_hash)) {
		if (isHoldingBlock)
		{
			SoundManager::PlaySE("BlockRotate", 1.0f, false);
		}
		holderTransform->Rotate(0.0f, 90.0f, 0.0f);

	}


	Vec2 dir = GetFlattenedDirection(playerTransform->GetQuaternion());
	Vector3 blockSize = m_pBlockObject->GetSize();
	blockSize = m_pBlockObject->GetTransform()->GetQuaternion() * blockSize;
	blockSize.x = std::fabsf(blockSize.x);
	blockSize.y = std::fabsf(blockSize.y);
	blockSize.z = std::fabsf(blockSize.z);
	Vec2 placeCursorOffsetXZ = -CalcSpacedRectPos(Vec2{ blockSize.x, blockSize.z }, 1.0f, dir);

	Vector3 placeCursorOffset{ placeCursorOffsetXZ.x, 0.0f, placeCursorOffsetXZ.y };
	placeCursorOffset -= blockOffset + Vector3{ 0.0f, 0.5f, 0.0f };

	Vector3 placeCursorPos = blockTransform->GetPosition() + placeCursorOffset;

	//ブロックの底面分だけ位置を補正（地面に接地させるためのオフセット）
	float blockGroundYOffset = m_pBlockObject->GetGroundYOffset();
	placeCursorPos.y += blockGroundYOffset;

	Vector3 placeCursorPosXZ = placeCursorPos;
	placeCursorPosXZ.y = blockGroundYOffset;

	//プレイヤーからのブロックを取るための相対座標
	Vector3 removeCursorOffset{ 0.0f, 0.5f, -1.15f };

	Vector3 removeCursorPos = playerTransform->GetPosition() + playerTransform->GetQuaternion() * removeCursorOffset;
	Vector3 removeCursorPosXZ = removeCursorPos;
	removeCursorPosXZ.y = 0.5f;

	//毎回書くと長くなるので格納してわかりやすくする。
	GridField* pGridField = GameState::GetInstance()->GetGridField();

	//ブロックが存在しなかったら
	if (m_pBlockObject->GetBlockSet().blocks.empty()) {

		//配置カーソルの位置を初期化する。
		pGridField->ResetPlaceCursor();

		//取得カーソルの位置を設定する。
		pGridField->SetRemoveCursor(removeCursorPos);

		m_pPlaceCursor->GetGameObject()->SetActive(false);

		//毎回書くと長くなるので格納してわかりやすくする。
		auto pWorldBlocks = GameState::GetInstance()->GetWorldBlocks();

		
		bool isYetSelect = true;
		BlockObject* pRemoveBlock = nullptr;

		for (auto&& pBlock : pWorldBlocks) {

			if (pBlock->IsInside(removeCursorPosXZ)) {

				pBlock->SetSelect(isYetSelect);

				if (isYetSelect) {
					pRemoveBlock = pBlock.Get();
				}
				
				isYetSelect = false;
			}
			else {

				pBlock->SetSelect(false);

			}
		}

		if (isYetSelect) {
			for (auto&& pBlock : pWorldBlocks) {

				if (pBlock->IsInside(removeCursorPosXZ, 1.0f)) {

					pBlock->SetSelect(isYetSelect);

					if (isYetSelect) {
						pRemoveBlock = pBlock.Get();
					}

					isYetSelect = false;
				}
				else {

					pBlock->SetSelect(false);

				}
			}
		}

		if (InputManager::CurrentInputSystem().GetButtonDown("PlaceAndRemove"_hash)) {

			//trueが帰ってきたらグリッド内
			if (pGridField->IsInside(removeCursorPosXZ)) {
				auto blockData = pGridField->RemoveBlock();
				if (blockData.has_value()) {

					SetBlockSet(blockData->blockSet);
					m_pBlockObject->SetModel(blockData->modelPath);
					holderTransform->SetQuaternion(blockData->rotation);

					// SE再生
					SoundManager::PlaySE("PutBox", 1.0f, false);
					m_pRenderer->PlayBlend(m_motuAnime, g_blendTime, true);
				}

			}
			else {//faleが帰ってきたら

				if (pRemoveBlock) {
					SetBlockSet(pRemoveBlock->GetBlockSet());
					m_pBlockObject->SetModel(pRemoveBlock->GetModelPath());
					holderTransform->SetQuaternion(pRemoveBlock->GetTransform()->GetQuaternion());

					GameState::GetInstance()->RemoveWorldBlock(pRemoveBlock);

					pRemoveBlock->GetGameObject()->Destroy();

					// SE再生
					SoundManager::PlaySE("PutBox", 1.0f, false);
					m_pRenderer->PlayBlend(m_motuAnime, g_blendTime, true);
				}
			}
		}
	}
	else {

		pGridField->ResetRemoveCursor();
		pGridField->SetPlaceCursor(m_pBlockObject->GetBlockSet(), placeCursorPos, blockTransform->GetQuaternion(), m_pBlockObject->GetModelPath());

		bool isOverlapGridField = pGridField->IsOverlap(m_pBlockObject->GetBlockSet(), placeCursorPosXZ, blockTransform->GetQuaternion());
		bool isOutsideStage = false;
		//ステージ外に出ていないかの判定
		Vector3 stageSize = GameState::GetInstance()->GetStageSize();
		stageSize *= 0.5f;
		Vector3 stagePos = GameState::GetInstance()->GetStagePos();
		for (auto&& vtx : m_pBlockObject->GetBlockVertices()) {
			Vector3 worldPos = blockTransform->GetQuaternion() * vtx + placeCursorPosXZ;
			if (worldPos.x < stagePos.x - stageSize.x || worldPos.x > stagePos.x + stageSize.x ||
				worldPos.y < stagePos.y - stageSize.y || worldPos.y > stagePos.y + stageSize.y ||
				worldPos.z < stagePos.z - stageSize.z || worldPos.z > stagePos.z + stageSize.z) {
				isOutsideStage = true;
				break;
			}
		}

		if (!isOverlapGridField && !isOutsideStage) {
			// カーソルを表示する
			m_pPlaceCursor->GetGameObject()->SetActive(true);
			m_pPlaceCursor->SetBlockSet(m_pBlockObject->GetBlockSet());
			m_pPlaceCursor->SetPlaceable(CanPlaceWorld(placeCursorPosXZ));
			m_pPlaceCursor->GetTransform()->SetPosition(placeCursorPosXZ);
			m_pPlaceCursor->GetTransform()->SetQuaternion(m_pBlockObject->GetTransform()->GetQuaternion());
		}
		else {
			// カーソルはグリッドフィールドに任せる
			m_pPlaceCursor->GetGameObject()->SetActive(false);
		}

		if (InputManager::CurrentInputSystem().GetButtonDown("PlaceAndRemove"_hash) && !isOutsideStage) {

			//グリッド内かどうかの判定
			if (isOverlapGridField)
			{
				//ブロックを初期化してなくす
				if (pGridField->PlaceBlock()) {

					m_pBlockObject->SetBlockSet(BlockSetData{});

					if (pGridField->IsCorrect())
					{
						// SE再生
						SoundManager::PlaySE("Correct", 1.0f, false);
					}
					else
					{
						SoundManager::PlaySE("Wrong", 1.0f, false);
					}
					
					m_pRenderer->PlayBlend(m_okuAnime, g_blendTime, true);

					SetBlockSet(BlockSetData{});
					m_pBlockObject->SetModel("");
				}
			}
			else {
				//グリッド外の場合はワールドに配置する。
				PlaceBlockWorld(placeCursorPosXZ);
			}
		}
	}
}


void PlayerBlockHandler::SetBlockSet(const BlockSetData& blockSet)
{
	m_pBlockObject->SetBlockSet(blockSet);
	m_pBlockObject->GetTransform()->SetPosition(m_pBlockObject->GetCenterGroundOffset(), Space::LOCAL);
}

void PlayerBlockHandler::PlaceBlockWorld(Vector3 placePos)
{
	if (!CanPlaceWorld(placePos)) {
		return;
	}
	auto obj = SceneManager::GetActiveScene()->CreateGameObject();
	auto component = obj->AddComponent<BlockObject>();
	auto transform = obj->GetTransform();
	transform->SetPosition(placePos);
	transform->SetQuaternion(m_pBlockObject->GetTransform()->GetQuaternion());
	component->SetUseCollider(true);
	component->SetBlockSet(m_pBlockObject->GetBlockSet());
	component->SetModel(m_pBlockObject->GetModelPath());
	GameState::GetInstance()->AppendWorldBlock(component);
	//使った頭上のブロックは初期化
	m_pBlockObject->SetBlockSet(BlockSetData{});

	// SE再生
	SoundManager::PlaySE("OutGrid", 1.0f, false);
	m_pRenderer->PlayBlend(m_okuAnime, g_blendTime, true);
	SetBlockSet(BlockSetData{});
	m_pBlockObject->SetModel("");

	component->CreateWorldPlaceEffect();
}

bool PlayerBlockHandler::CanPlaceWorld(Vector3 placePos)
{
	for (auto&& pBlock : GameState::GetInstance()->GetWorldBlocks()) {
		for (auto&& vtx : m_pBlockObject->GetBlockVertices()) {
			if (pBlock->IsInside(m_pBlockObject->GetTransform()->GetQuaternion() * vtx + placePos)) {
				return false;
			}
		}
	}
	return true;
}


Vec2 CalcSpacedRectPos(const Vec2& rectSize, float distance, const Vec2& direction)
{
	float halfW = rectSize.x * 0.5f;
	float halfH = rectSize.y * 0.5f;

	float distX = (std::abs(direction.x) > direction.EpsilonScalar) ? (halfW / std::abs(direction.x)) : std::numeric_limits<float>::max();
	float distY = (std::abs(direction.y) > direction.EpsilonScalar) ? (halfH / std::abs(direction.y)) : std::numeric_limits<float>::max();

	float distToRectEdge = std::min(distX, distY);

	float totalDist = distance + distToRectEdge;

	return direction * totalDist;
}