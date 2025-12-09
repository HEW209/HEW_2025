/*****************************************************************//**
 * @file   Component.h
 * @brief  コンポーネントの基底クラス
 *
 * @author 石田怜
 * @date   2025/09/10
 *********************************************************************/
#pragma once

#include "Object.h"

class GameObject;
class Transform;

/**
 * @brief コンポーネントの基底クラス
 */
class Component : public Object
{
	friend class GameObject;

public:
	Component();
	virtual ~Component() = default;

	/**
	 * @brief ★親ゲームオブジェクトを取得する
	 * @return 親ゲームオブジェクトへのポインタ
	 */
	GameObject* GetGameObject() const
	{
		return m_pGameObject;
	}

	/**
	 * @brief ★親ゲームオブジェクトが持つTransformコンポーネントを取得する
	 * @return 親ゲームオブジェクトが持つTransformコンポーネントへのポインタ
	 */
	Transform* GetTransform() const
	{
		return m_pTransform;
	}

	/**
	 * @brief このコンポーネントの有効状態を設定する
	 * @param enabled 設定する有効状態
	 */
	void SetEnabled(bool enabled)
	{
		m_enabled = enabled;
	}

	/**
	 * @brief このコンポーネントの有効状態を取得する
	 * @return このコンポーネントの有効状態
	 */
	bool IsEnabled() const
	{
		return m_enabled;
	}

	/**
	* @brief 開始処理呼び出し状態を取得する
	* @return 開始処理がすでに呼ばれていればtrueを返す
	*/
	bool IsStarted() const
	{
		return m_started;
	}

	/**
	 * @brief ★このコンポーネントを削除する
	 * @details 実際の削除は全ての更新処理の終了時に安全に行われます
	 */
	void Destroy() override
	{
		m_destroyed = true;
	}

private:
	/// 親ゲームオブジェクトへのポインタ
	GameObject* m_pGameObject;

	/// 親ゲームオブジェクトが持つTransformコンポーネントへのポインタ
	Transform* m_pTransform;

	/// 有効状態フラグ
	bool m_enabled;

	/// 削除フラグ
	bool m_destroyed;

	/// Start呼び出しフラグ
	bool m_started;

	/**
	 * @brief 生成時に呼び出される処理
	 */
	virtual void Awake() {}

	/**
	 * @brief ★更新処理の前に一度だけ呼ばれる処理
	 */
	virtual void Start() {}

	/**
	 * @brief ★毎フレーム呼ばれる更新処理
	 */
	virtual void Update() {}

	/**
	 * @brief Updateの後に呼ばれる更新処理
	 */
	virtual void LateUpdate() {}

	/**
	 * @brief 削除時に呼び出される処理
	 */
	virtual void OnDestroy() {}

	/**
	 * @brief 親ゲームオブジェクトを設定する
	 * @param pGameObject 親ゲームオブジェクトへのポインタ
	 */
	void SetGameObject(GameObject* pGameObject);

	/**
	 * @brief 開始処理呼び出しフラグをオンにする
	 */
	void SetStarted()
	{
		m_started = true;
	}

	/**
	 * @brief このコンポーネントの削除フラグを取得する
	 * @return このコンポーネントの削除フラグ
	 */
	bool IsDestroyed() const
	{
		return m_destroyed;
	}
};
