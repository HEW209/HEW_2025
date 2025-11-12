/*****************************************************************//**
 * @file   Renderer2D.h
 * @brief  2D描画コンポーネントの基底クラス
 *
 * @author 石田怜
 * @date   2025/09/22
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>

/**
 * @brief 2D描画コンポーネントの基底クラス
 */
class Renderer2D : public Component
{
public:
	Renderer2D();
	virtual ~Renderer2D();

	/**
	 * @brief 描画処理
	 */
	virtual void Draw() = 0;

	/**
	 * @brief 現在の描画優先度を取得する
	 * @return 描画優先度 (大きいほど上に表示される)
	 */
	int GetOrder() const
	{
		return m_drawOrder;
	}

	/**
	 * @brief 描画優先度を設定する
	 * @param order 描画優先度 (大きいほど上に表示される)
	 */
	void SetOrder(int order)
	{
		m_drawOrder = order;
	}

private:
	/// 描画優先度
	int m_drawOrder;
};