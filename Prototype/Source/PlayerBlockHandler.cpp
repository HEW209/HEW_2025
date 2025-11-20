#include "PlayerBlockHandler.h"
#include "GameState.h"
#include "InputSystem.h"


void PlayerBlockHandler::Start()
{
	
}

void PlayerBlockHandler::Update()
{
	auto blockTransform = m_pBlockObject->GetTransform();
	auto playerTransform = GetTransform();



	

	if (InputSystem::GetButtonDown("RotateBlockRight"_hash)) {
		blockTransform->SetQuaternion(Quaternion::Euler(0.0f, 0.0f, -90.0f) * blockTransform->GetQuaternion());

	}
	if (InputSystem::GetButtonDown("RotateBlockLeft"_hash)) {
		blockTransform->SetQuaternion(Quaternion::Euler(0.0f, 0.0f, 90.0f) * blockTransform->GetQuaternion());

	}
	if (InputSystem::GetButtonDown("RotateBlockUp"_hash)) {
		blockTransform->SetQuaternion(Quaternion::Euler(90.0f, 0.0f, 0.0f) * blockTransform->GetQuaternion());

	}
	if (InputSystem::GetButtonDown("RotateBlockDown"_hash)) {
		blockTransform->SetQuaternion(Quaternion::Euler(-90.0f, 0.0f, 0.0f) * blockTransform->GetQuaternion());

	}


	if (Input::GetKeyDown(KeyCode::KEY_1)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(1);
		data.blocks[0] = { 0, 0, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_2)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(2);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_3)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(3);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_4)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(4);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_5)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(4);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 0, 0, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_6)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(5);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_7)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(6);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 0, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_8)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(6);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 1, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_9)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(7);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 0, 1 };
		data.blocks[6] = { 0, 1, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (Input::GetKeyDown(KeyCode::KEY_0)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(8);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 0, 1 };
		data.blocks[6] = { 0, 1, 1 };
		data.blocks[7] = { 1, 1, 1 };
		m_pBlockObject->SetBlockSet(data);
	}


	//プレイヤーからブロックを置く位置を決めるための相対オフセット（プレイヤー前方1.5m）
	Vector3 placeCursorOffset{ 0.0f, 0.0f, -1.5f };

	//プレイヤーのクォータニオンを、相対オフセット方向に適用
	//「プレイヤーの向いている方向に応じて、前方1.5mの位置」を求める。
	Vector3 placeCursorPos = playerTransform->GetPosition() + playerTransform->GetQuaternion() * placeCursorOffset; 

	//ブロックの底面分だけ位置を補正（地面に接地させるためのオフセット）
	placeCursorPos += m_pBlockObject->GetGroundOffset();

	//プレイヤーからのブロックを取るための相対座標
	Vector3 removeCursorOffset{ 0.0f, 0.5f, -1.5f };

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

	
	Vector3 blockOffset{ 0.0f, 2.0f, 0.0f };
	//地面に設置させるために計算
	blockOffset += m_pBlockObject->GetGroundOffset();
	
	blockTransform->SetPosition(playerTransform->GetPosition() + blockOffset);



}

void PlayerBlockHandler::TryPlaceBlock()
{

	CheckPlaceBlock();
}

void PlayerBlockHandler::SetBlockObject(BlockObject* pBlockObject)
{
	m_pBlockObject = pBlockObject;
}

bool PlayerBlockHandler::CheckPlaceBlock()
{
	
	return false;
}
