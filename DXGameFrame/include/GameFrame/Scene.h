/*****************************************************************//**
 * @file   Scene.h
 * @brief  シーンの基底クラス
 * 
 * @author 石田怜
 * @date   2025/09/11
 *********************************************************************/
#pragma once

#include "GameObject.h"
#include <typeindex>
#include <unordered_map>

class SceneManager;

/**
 * @brief シーンの基底クラス
 */
class Scene
{
	friend class SceneManager;

public:
	Scene() = default;
	virtual ~Scene();

	/**
	 * @brief ★このシーンにゲームオブジェクトを生成する
	 * @return 生成したゲームオブジェクトへのポインタ
	 */
	GameObject* CreateGameObject();

protected:
	/**
	 * @brief ★シーンの初期化を行う
	 */
	virtual void Init() = 0;
	
	/**
	 * @brief ★シーンの終了処理を行う
	 */
	virtual void Uninit() {}

private:
	/// シーン内ゲームオブジェクトのリスト
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;

	/**
	 * @brief 全てのゲームオブジェクトの更新処理を呼び出す
	 */
	void Update();

	/**
	 * @brief 全てのゲームオブジェクトの削除を適用
	 */
	void ApplyDestroyGameObject();
};