/*****************************************************************//**
 * @file   GameObject.h
 * @brief  ゲーム内のオブジェクト
 *
 * コンポーネントを保持し、シーン上での状態管理や更新処理を行う
 * コンポーネントの追加・取得・削除などを行う
 *
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "Component.h"
#include <vector>
#include <memory>

class Scene;

/**
 * @brief ゲーム内のオブジェクト
 */
class GameObject : public Object
{
	friend class Scene;

public:
	GameObject(Scene* pScene);
	~GameObject();

	/**
	 * @brief コンポーネントを取得する
	 * @note 継承元のクラスそ指定した場合、継承先のクラスも検索対象になります
	 * 例：Colliderコンポーネントを指定した場合、BoxColliderやSphereColliderも検索対象になる
	 * @return 取得したコンポーネントへのポインタ
	 * 指定したコンポーネントが見つからなかった場合はnullptrを返す
	 */
	template<typename T>
	T* GetComponent();

	/**
	 * @brief コンポーネントを追加する
	 * @return 追加したコンポーネントへのポインタ
	 */
	template<typename T>
	T* AddComponent();

	/**
	 * @brief コンポーネントを削除する
	 * 削除対象が複数存在する場合は、最初に見つかったものを削除する
	 * @note 継承元のクラスそ指定した場合、継承先のクラスも削除対象になります
	 * 例：Colliderコンポーネントを指定した場合、BoxColliderやSphereColliderも削除対象になる
	 * @return 削除できた場合trueを返す
	 */
	template<typename T>
	bool RemoveComponent();

	/**
	 * @brief Transformコンポーネントを取得する
	 * @return このゲームオブジェクトが持つTransformコンポーネントへのポインタ
	 */
	Transform* GetTransform() const;

	/**
	 * @brief このゲームオブジェクトの有効状態を設定する
	 * @param isActive 設定する有効状態
	 */
	void SetActive(bool isActive);

	/**
	 * @brief このゲームオブジェクト自身の有効状態を取得する
	 * @return このゲームオブジェクト自身の有効状態
	 */
	bool IsActiveSelf() const;

	/**
	 * @brief 親子関係を考慮した有効状態を取得する
	 * @return ヒエラルキー上の有効状態
	 */
	bool IsActiveHierarchy() const;

	/**
	 * @brief このゲームオブジェクトを削除する
	 * @details 実際の削除は全ての更新処理の終了時に安全に行われます
	 */
	void Destroy();

private:
	/// このゲームオブジェクトが存在するシーンへのポインタ
	Scene* m_pScene;

	/// Transformコンポーネントへのポインタ
	Transform* m_pTransform;

	/// このゲームオブジェクトが持つコンポーネントのリスト
	std::vector<std::unique_ptr<Component>> m_components;

	/// 有効状態フラグ
	bool m_isActive;

	/// 削除フラグ
	bool m_destroyed;

	/**
	 * @brief 未実行のコンポーネントの開始処理を呼び出す
	 */
	void StartAllComponent();

	/**
	 * @brief 全てのコンポーネントの更新処理を行う
	 */
	void UpdateAllComponent();

	/**
	 * @brief 全てのコンポーネントの遅延更新処理を行う
	 */
	void LateUpdateAllComponent();

	/**
	 * @brief 全てのコンポーネントの削除時処理を呼び出す
	 */
	void OnDestroy();

	/**
	* @brief 全てのコンポーネントの削除を適用
	*/
	void ApplyRemoveComponent();

	/**
	 * @brief このゲームオブジェクトの削除フラグを取得する
	 * @return このゲームオブジェクトの削除フラグ
	 */
	bool IsDestroyed() const;
};

template<typename T>
inline T* GameObject::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value,
		"GetComponentに無効なクラスが指定されました");

	for (auto& com : m_components)
	{
		//型キャスト可能か調べる
		if (auto ptr = dynamic_cast<T*>(com.get()))
		{
			return ptr;
		}
	}

	return nullptr;
}

template<typename T>
inline T* GameObject::AddComponent()
{
	static_assert(std::is_base_of<Component, T>::value,
		"AddComponentに無効なクラスが指定されました");

	//コンポーネント追加
	auto com = std::make_unique<T>();
	T* ptr = com.get();
	m_components.emplace_back(std::move(com));

	//初期設定
	Component* comPtr = ptr;
	comPtr->SetGameObject(this);
	comPtr->Awake();

	return ptr;
}

template<typename T>
inline bool GameObject::RemoveComponent()
{
	static_assert(std::is_base_of<Component, T>::value,
		"RemoveComponentに無効なクラスが指定されました");

	for (auto& com : m_components)
	{
		//型キャスト可能か調べる
		if (auto ptr = dynamic_cast<T*>(com.get()))
		{
			if (ptr->IsDestroyed())
				continue;

			ptr->Destroy();
			return true;
		}
	}

	return false;
}
