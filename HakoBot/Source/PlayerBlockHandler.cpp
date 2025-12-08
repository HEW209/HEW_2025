#include "PlayerBlockHandler.h"
#include "GameState.h"
#include "InputSystem.h"


PlayerBlockHandler::PlayerBlockHandler()
{
}

void PlayerBlockHandler::Awake()
{
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		m_pBlockObject = obj->AddComponent<BlockObject>();
		m_pBlockObject->SetUseCollider(false);
	}

	m_pBlockHolder = SceneManager::GetActiveScene()->CreateGameObject();
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


	//プレイヤーからブロックを置く位置を決めるための相対オフセット（プレイヤー前方1.5m）
	//Vector3 blockSize = playerTransform->GetQuaternion() * m_pBlockObject->GetSize();
	//Vector3 placeCursorOffset{ 0.0f, 0.0f, -std::abs(blockSize.z) * 0.5f - 1.0f };
	Vector3 placeCursorOffset{ 0.0f, 0.0f, -1.5f };

	placeCursorOffset -= blockOffset;

	placeCursorOffset += blockTransform->GetPosition();

	playerTransform->GetQuaternion() * Vec3 { 0.0f, 0.0f, 1.0f };

	placeCursorOffset += m_pBlockObject->GetSize();

	//プレイヤーのクォータニオンを、相対オフセット方向に適用
	//「プレイヤーの向いている方向に応じて、前方1.5mの位置」を求める。
	Vector3 placeCursorPos = playerTransform->GetPosition() + playerTransform->GetQuaternion() * placeCursorOffset; 

	//ブロックの底面分だけ位置を補正（地面に接地させるためのオフセット）
	placeCursorPos += m_pBlockObject->GetGroundOffset();

	//プレイヤーからのブロックを取るための相対座標
	Vector3 removeCursorOffset{ 0.0f, 0.5f, -1.2f };

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

					m_pBlockObject->SetBlockSet(blockData->blockSet);
					blockTransform->SetQuaternion(blockData->rotation);

				}

			}
			else {//faleが帰ってきたら

				auto pWorldBlocks = GameState::GetInstance()->GetWorldBlocks();

				for (auto&& pBlock : pWorldBlocks) {
					
					if (pBlock->IsInside(removeCursorPos)) {
						m_pBlockObject->SetBlockSet(pBlock->GetBlockSet());
						blockTransform->SetQuaternion(pBlock->GetTransform()->GetQuaternion());

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
		pGridField->SetPlaceCursor(m_pBlockObject->GetBlockSet(), placeCursorPos, blockTransform->GetQuaternion());

		if (InputSystem::GetButtonDown("PlaceAndRemove"_hash)) {

			//グリッド内かどうかの判定
			if (pGridField->IsOverlap(m_pBlockObject->GetBlockSet(), placeCursorPos, blockTransform->GetQuaternion()))
			{
				//ブロックを初期化してなくす
				if (pGridField->PlaceBlock()) {

					m_pBlockObject->SetBlockSet(BlockSetData{});

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
				GameState::GetInstance()->AppendWorldBlock(component);
				//使った頭上のブロックは初期化
				m_pBlockObject->SetBlockSet(BlockSetData{});
			}

		}
	}
}