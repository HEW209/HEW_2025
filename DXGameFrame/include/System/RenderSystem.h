/*****************************************************************//**
 * @file   RenderSystem.h
 * @brief  全ての描画処理を管理するシステム
 *
 * @author 石田怜
 * @date   2025/09/22
 *********************************************************************/
#pragma once

#include <vector>
#include <Component/Renderer.h>
#include <Component/Renderer2D.h>
#include <Utility/Color.h>

/**
 * @brief 全ての描画処理を管理するシステム
 */
class RenderSystem
{
public:
	/**
	 * @brief 全ての描画コンポーネントの描画処理を呼び出す
	 */
	void DrawAll();

	/**
	 * @brief 3D描画コンポーネントを登録する
	 * @param pRenderer 登録する3D描画コンポーネント
	 */
	void Register(Renderer* pRenderer);

	/**
	 * @brief 3D描画コンポーネントの登録を削除する
	 * @param pRenderer 登録削除する3D描画コンポーネント
	 */
	void Unregister(Renderer* pRenderer);

	/**
	 * @brief 2D描画コンポーネントを登録する
	 * @param pRenderer2D 登録する2D描画コンポーネント
	 */
	void Register2D(Renderer2D* pRenderer2D);

	/**
	 * @brief 2D描画コンポーネントの登録を削除する
	 * @param pRenderer2D 登録削除する2D描画コンポーネント
	 */
	void Unregister2D(Renderer2D* pRenderer2D);

private:
	RenderSystem();

	/// 3D描画コンポーネントのリスト
	std::vector<Renderer*> m_pRendererComponents;

	/// 2D描画コンポーネントのリスト
	std::vector<Renderer2D*> m_pRenderer2DComponents;

	/// 画面クリア色
	Color m_clearColor;

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return RenderSystemインスタンスへの参照
	 */
	static RenderSystem& Instance()
	{
		static RenderSystem s_instance;
		return s_instance;
	}
};