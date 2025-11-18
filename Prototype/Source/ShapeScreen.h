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
	ShapeScreen() = default;
	~ShapeScreen() = default;

	void OnDestroy() override;

	/// @brief クリア形状を設定
	void SetClearShape(const ShapeType& shape);

	/// @brief 現在の形状を設定
	void SetCurrentShape(const ShapeType& shape);

	/// @brief クリアしているか
	bool IsClear();

private:
	void UpdateClearShapeBlocks();
	void UpdateCurrentShapeBlocks();

	std::vector<ObjPtr<GameObject>> m_pCurrentShapeBlocks;
	std::vector<ObjPtr<GameObject>> m_pClearShapeBlocks;
	ShapeType m_clearShape;
	ShapeType m_currentShape;
};