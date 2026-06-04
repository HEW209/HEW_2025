/*****************************************************************//**
 * @file   Renderer.h
 * @brief  3D描画コンポーネントの基底クラス
 *
 * @author 石田怜
 * @date   2025/11/24
 * 
 * @date   2025/12/28 [芝晃佑]	シャドウ描画対応
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>

/**
 * @brief 3D描画コンポーネントの基底クラス
 */
class Renderer : public Component
{
public:
	Renderer();
	virtual ~Renderer();

	/**
	 * @brief 描画処理
	 */
	virtual void Draw() = 0;

	/**
	 * @brief 影描画処理
	 */
	virtual void DrawShadow() = 0;

	/**
	 * @brief 深度描画処理
	 */
	virtual void DrawDepth() = 0;

	/**
	 * @brief 透過オブジェクトかを設定する
	 * @param transparent 透過オブジェクトフラグ
	 */
	void SetTransparent(bool transparent);

	/**
	 * @brief 透過オブジェクト設定を取得する
	 * @return 透過オブジェクトならtrueを返す
	 */
	bool IsTransparent();

	/**
	 * @brief 透過オブジェクト（グループ）かを設定する
	 * @brief グループ透過オブジェクト同士は互いを透過しない
	 * @param transparent 透過オブジェクトフラグ
	 */
	void SetGroupTransparent(bool transparent);

	/**
	 * @brief グループ透過オブジェクト設定を取得する
	 * @return グループ透過オブジェクトならtrueを返す
	 */
	bool IsGroupTransparent();

private:
	bool m_isTransparent;
	bool m_isGroupTransparent;
};