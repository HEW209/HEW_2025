/*****************************************************************//**
 * @file   RenderSystem.h
 * @brief  全ての描画処理を管理するシステム
 *
 * @author 石田怜
 * @date   2025/11/24
 * 
 * @date   2025/12/28 [芝晃佑]	シャドウ描画対応
 *********************************************************************/
#pragma once

#include <vector>
#include <Component/Renderer.h>
#include <Component/Renderer2D.h>
#include <Component/EffectRenderer.h>
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

	/**
	 * @brief 3D描画コンポーネントの影描画を登録する
	 * @param pRenderer 登録する3D描画コンポーネント
	 */
	void RegisterShadow(Renderer* pRenderer);

	/**
	 * @brief 3D描画コンポーネントの影描画の登録を削除する
	 * @param pRenderer 登録削除する3D描画コンポーネント
	 */
	void UnregisterShadow(Renderer* pRenderer);

	/**
	 * @brief エフェクト描画コンポーネントを登録する
	 * @param pRenderer 登録するエフェクト描画コンポーネント
	 */
	void RegisterEffect(EffectRenderer* pRenderer);

	/**
	 * @brief エフェクト描画コンポーネントの登録を削除する
	 * @param pRenderer 登録削除するエフェクト描画コンポーネント
	 */
	void UnregisterEffect(EffectRenderer* pRenderer);

	/**
	 * @brief 画面クリア色を設定する
	 * @param color 画面クリア色
	 */
	void SetClearColor(Color color);

private:
	RenderSystem();

	/// 3D描画コンポーネントのリスト
	std::vector<Renderer*> m_pRendererComponents;

	/// 2D描画コンポーネントのリスト
	std::vector<Renderer2D*> m_pRenderer2DComponents;

	/// 3D影描画コンポーネントのリスト
	std::vector<Renderer*> m_pShadowRendererComponents;

	/// エフェクト描画コンポーネントのリスト
	std::vector<EffectRenderer*> m_pEffectRendererComponents;

	/// 画面クリア色
	Color m_clearColor;

	/**
	 * @brief 全ての背景2D描画コンポーネントを描画する
	 */
	void DrawALL2DBackGround();

	/**
	 * @brief 全ての3D描画コンポーネントを描画する
	 */
	void DrawAll3D();

	/**
	 * @brief 全ての2D描画コンポーネントを描画する
	 */
	void DrawAll2D();

	/**
	 * @brief 全てのエフェクト描画コンポーネントを描画する
	 */
	void DrawAllEffect();

	/**
	 * @brief 全てのデバック機能の描画を行う
	 */
	void DrawDebugUI();

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return RenderSystemインスタンスへの参照
	 */
	static RenderSystem& Instance();
};