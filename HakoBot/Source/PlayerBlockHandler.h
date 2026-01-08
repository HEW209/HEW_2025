/*********************************************************************
 * @file   PlayerBlockHandler.h
 * @brief  プレイヤーブロックを操作するためのコンポーネントクラス
 *
 * @author 長尾潤弥
 * @date   2025/11/17
 *********************************************************************/

#pragma once

#include <DXGameFrame.h>
#include "BlockObject.h"
#include "GridField.h"

class PlayerBlockHandler : public Component
{
public:
	PlayerBlockHandler();
	
	void Awake() override;
	void LateUpdate() override;


	bool HasBlock()
	{
		return !m_pBlockObject->GetBlockSet().blocks.empty();
	}

private:
	void SetBlockSet(const BlockSetData& blockSet);

	void PlaceBlockWorld(Vector3 placePos);

	bool CanPlaceWorld(Vector3 placePos);

	//持ってるブロックオブジェクト
	ObjPtr<BlockObject> m_pBlockObject;

	//ブロックの回転情報を持つ、親オブジェクト
	ObjPtr<GameObject> m_pBlockHolder;

	ObjPtr<PlaceCursor> m_pPlaceCursor;

	ObjPtr<MeshRenderer> m_pRenderer;
	MeshGroup::AnimeNo m_motuAnime;
	MeshGroup::AnimeNo m_okuAnime;
};