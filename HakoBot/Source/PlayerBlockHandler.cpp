#include "PlayerBlockHandler.h"

#include "GameState.h"
#include "InputSystem.h"
#include "VecUtil.h"


Vec2 CalcSpacedRectPos(const Vec2& rectSize, float distance, const Vec2& direction);


PlayerBlockHandler::PlayerBlockHandler()
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
}

void PlayerBlockHandler::Update()
{
	auto blockTransform = m_pBlockObject->GetTransform();
	auto playerTransform = GetTransform();
	auto holderTransform = m_pBlockHolder->GetTransform();


	Vector3 blockOffset{ 0.0f, 2.0f, 0.0f };
	//地面に設置させるために計算
	blockOffset += m_pBlockObject->GetGroundOffset();

	holderTransform->SetPosition(playerTransform->GetPosition() + blockOffset);


	if (InputSystem::GetButtonDown("RotateBlockRight"_hash)) {
		holderTransform->Rotate(0.0f, -90.0f, 0.0f);

	}
	if (InputSystem::GetButtonDown("RotateBlockLeft"_hash)) {
		holderTransform->Rotate(0.0f, 90.0f, 0.0f);

	}


	Vec2 dir = GetFlattenedDirection(playerTransform->GetQuaternion());
	Vector3 blockSize = m_pBlockObject->GetSize();
	Vec2 placeCursorOffsetXZ = -CalcSpacedRectPos(Vec2{ blockSize.x, blockSize.z }, 1.0f, dir);

	Vector3 placeCursorOffset{ placeCursorOffsetXZ.x, 0.0f, placeCursorOffsetXZ.y };
	placeCursorOffset -= blockOffset + Vector3{ 0.0f, 0.5f, 0.0f };

	Vector3 placeCursorPos = blockTransform->GetPosition() + placeCursorOffset;

	//ブロックの底面分だけ位置を補正（地面に接地させるためのオフセット）
	placeCursorPos += m_pBlockObject->GetGroundOffset();

	//プレイヤーからのブロックを取るための相対座標
	Vector3 removeCursorOffset{ 0.0f, 0.5f, -1.35f };

	Vector3 removeCursorPos = playerTransform->GetPosition() + playerTransform->GetQuaternion() * removeCursorOffset;

	//毎回書くと長くなるので格納してわかりやすくする。
	GridField* pGridField = GameState::GetInstance()->GetGridField();

	//ブロックが存在しなかったら
	if (m_pBlockObject->GetBlockSet().blocks.empty()) {

		//配置カーソルの位置を初期化する。
		pGridField->ResetPlaceCursor();

		//取得カーソルの位置を設定する。
		pGridField->SetRemoveCursor(removeCursorPos);

		//毎回書くと長くなるので格納してわかりやすくする。
		auto pWorldBlocks = GameState::GetInstance()->GetWorldBlocks();

		
		bool isYetSelect = true;

		for (auto&& pBlock : pWorldBlocks) {

			if (pBlock->IsInside(removeCursorPos)) {

				pBlock->SetSelect(isYetSelect);
				
				isYetSelect = false;
			}
			else {

				pBlock->SetSelect(false);

			}
		}

		if (InputSystem::GetButtonDown("PlaceAndRemove"_hash)) {

			//trueが帰ってきたらグリッド内
			if (pGridField->IsInside(removeCursorPos)) {


				auto blockData = pGridField->RemoveBlock();
				if (blockData.has_value()) {

					SetBlockSet(blockData->blockSet);
					m_pBlockObject->SetModel(blockData->modelPath);
					holderTransform->SetQuaternion(blockData->rotation);

				}

			}
			else {//faleが帰ってきたら

				auto pWorldBlocks = GameState::GetInstance()->GetWorldBlocks();

				for (auto&& pBlock : pWorldBlocks) {
					
					if (pBlock->IsInside(removeCursorPos)) {
						SetBlockSet(pBlock->GetBlockSet());
						m_pBlockObject->SetModel(pBlock->GetModelPath());
						holderTransform->SetQuaternion(pBlock->GetTransform()->GetQuaternion());

						GameState::GetInstance()->RemoveWorldBlock(pBlock.Get());

						pBlock->GetGameObject()->Destroy();
						break;
					}
				}
			}
		}
	}
	else {

		pGridField->ResetRemoveCursor();
		pGridField->SetPlaceCursor(m_pBlockObject->GetBlockSet(), placeCursorPos, blockTransform->GetQuaternion(), m_pBlockObject->GetModelPath());

		if (InputSystem::GetButtonDown("PlaceAndRemove"_hash)) {

			//グリッド内かどうかの判定
			if (pGridField->IsOverlap(m_pBlockObject->GetBlockSet(), placeCursorPos, blockTransform->GetQuaternion()))
			{
				//ブロックを初期化してなくす
				if (pGridField->PlaceBlock()) {

					SetBlockSet(BlockSetData{});
					m_pBlockObject->SetModel("");
				}
			}
			else {
				//グリッド外の場合はワールドに配置する。

				auto obj = SceneManager::GetActiveScene()->CreateGameObject();
				auto component = obj->AddComponent<BlockObject>();
				auto transform = obj->GetTransform();
				transform->SetPosition(placeCursorPos);
				transform->SetQuaternion(blockTransform->GetQuaternion());
				component->SetUseCollider(true);
				component->SetBlockSet(m_pBlockObject->GetBlockSet());
				component->SetModel(m_pBlockObject->GetModelPath());
				GameState::GetInstance()->AppendWorldBlock(component);
				//使った頭上のブロックは初期化
				SetBlockSet(BlockSetData{});
				m_pBlockObject->SetModel("");
			}

		}
	}
}


void PlayerBlockHandler::SetBlockSet(const BlockSetData& blockSet)
{
	m_pBlockObject->SetBlockSet(blockSet);
	m_pBlockObject->GetTransform()->SetPosition(m_pBlockObject->GetCenterGroundOffset(), Space::LOCAL);
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