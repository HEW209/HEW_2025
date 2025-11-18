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


 /**
  * @brief ブロックのコンポーネント
  */
class ShapeScreen : public Component
{
public:
	ShapeScreen() = default;
	~ShapeScreen() = default;

	void OnDestroy() override;

	void SetClearShape();
	void SetCurrentShape();

	/**
	 * @brief 指定したワールド座標がブロックの内側にあるか判定する
	 * @param worldPosition 判定したいワールド座標
	 * @return いずれかのブロックの内側にあれば true
	 */
	bool IsClear(const Vector3& worldPosition);

private:
	std::vector<ObjPtr<GameObject>> m_pBlocks;
};