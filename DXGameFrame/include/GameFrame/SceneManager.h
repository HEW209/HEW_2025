/*****************************************************************//**
 * @file   SceneManager.h
 * @brief  シーンの管理を行う
 *
 * @author 石田怜
 * @date   2025/11/24
 *********************************************************************/
#pragma once

#include "Scene.h"

/**
 * @brief シーンの管理を行う
 */
class SceneManager
{
public:
	/**
	 * @brief 初期化処理
	 * @param startScene 最初のシーン
	 */
	static void Init(std::unique_ptr<Scene> startScene);

	/**
	 * @brief 終了処理
	 */
	static void Uninit();

	/**
	 * @brief アクティブなシーンの時間を進める
	 * @param delta 前のフレームからの経過時間
	 */
	static void Execute(float delta);

	/**
	 * @brief シーンを変更する
	 * @details 同じフレームで複数回呼ばれた場合は最初に呼ばれたものが有効になります
	 * @param nextScene 次のシーン
	 */
	static void ChangeScene(std::unique_ptr<Scene> nextScene);

	/**
	 * @brief 現在のアクティブなシーンを取得する
	 * @return アクティブなシーンへのポインタ
	 */
	static Scene* GetActiveScene();

private:
	/// 現在のシーン
	static std::unique_ptr<Scene> s_activeScene;

	/// 次のシーン
	static std::unique_ptr<Scene> s_nextScene;

	/**
	 * @brief シーンの変更を適用する
	 */
	static void ApplyChangeScene();
};