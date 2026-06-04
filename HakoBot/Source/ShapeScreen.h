/*********************************************************************
 * @file   ShapeScreen.h
 * @brief  三面図を表示するスクリーンのコンポーネント
 *
 * @author 芝晃佑
 * @date   2025/11/18
 *********************************************************************/


#pragma once

#include <vector>

#include <DXGameFrame.h>

#include "GameTypes.h"


 /**
  * @brief ブロックのコンポーネント
  */
class ShapeScreen : public Component
{
public:
	ShapeScreen();
	~ShapeScreen() = default;

	void OnDestroy() override;

	/// @brief クリア形状を設定
	void SetClearShape(const ShapeType& shape, bool isFlip);

	/// @brief 現在の形状を設定
	void SetCurrentShape(const ShapeType& shape);

	/// @brief クリアしているか
	bool IsClear();

	/// @brief 透過設定を行う
	void SetTransparent(bool isHologram);

	/// @brief 透過設定を取得する
	bool IsTransparent() const { return m_isHologram; }

private:
	void UpdateClearShapeBlocks(bool isFlip);
	void UpdateCurrentShapeBlocks();

	struct ShapeBlock
	{
		ObjPtr<GameObject> pObj;
		ObjPtr<MeshRenderer> pRenderer;
	};

	std::vector<ShapeBlock> m_pShapeBlocks;
	ShapeType m_clearShape;
	ShapeType m_currentShape;

	bool m_isHologram;
};