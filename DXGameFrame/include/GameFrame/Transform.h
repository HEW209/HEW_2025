/******************************************************************//**
 * @file   Transform.h
 * @brief  オブジェクトの座標・スケール・回転を表す
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "Component.h"
#include <Utility/Vector3.h>
#include <Utility/Quaternion.h>

/// 座標空間
enum class Space
{
	WORLD,		// ワールド座標空間
	LOCAL		// ローカル座標空間
};

/**
 * @brief オブジェクトの座標・スケール・回転を表す
 */
class Transform : public Component
{
public:
	Transform();
	~Transform();

	/**
	 * @brief オブジェクトの座標を取得する
	 * @param space 取得に使用する座標空間
	 * @return 現在のオブジェクトの座標
	 */
	Vector3 GetPosition(Space space = Space::WORLD) const;

	/**
	 * @brief オブジェクトのスケールを取得する
	 * @return 現在のオブジェクトのローカルスケール
	 */
	Vector3 GetScale() const;

	/**
	 * @brief オブジェクトの回転をオイラー角で取得する
	 * @param space 取得に使用する座標空間
	 * @return 現在のオブジェクトの回転 (オイラー角)
	 */
	Vector3 GetEulerAngle(Space space = Space::WORLD) const;

	/**
	 * @brief オブジェクトの回転をクォータニオンで取得する
	 * @param space 取得に使用する座標空間
	 * @return 現在のオブジェクトの回転 (クォータニオン)
	 */
	Quaternion GetQuaternion(Space space = Space::WORLD) const;

	/**
	 * @brief オブジェクトの座標を設定する
	 * @param position 設定する座標
	 * @param space 設定に使用する座標空間
	 */
	void SetPosition(Vector3 position, Space space = Space::WORLD);

	/**
	 * @brief オブジェクトの座標を設定する
	 * @param x 設定するx座標
	 * @param y 設定するy座標
	 * @param z 設定するz座標
	 * @param space 設定に使用する座標空間
	 */
	void SetPosition(float x, float y, float z, Space space = Space::WORLD);

	/**
	 * @brief オブジェクトのスケールを設定する
	 * @param scale 設定するローカルスケール
	 */
	void SetScale(Vector3 scale);

	/**
	 * @brief オブジェクトのスケールを設定する
	 * @param x 設定するローカルxスケール
	 * @param y 設定するローカルyスケール
	 * @param z 設定するローカルzスケール
	 */
	void SetScale(float x, float y, float z);

	/**
	 * @brief オブジェクトの回転をオイラー角で設定する
	 * @param euler 設定する回転 (オイラー角)
	 * @param space 設定に使用する座標空間
	 */
	void SetEulerAngle(Vector3 euler, Space space = Space::WORLD);

	/**
	 * @brief オブジェクトの回転をオイラー角で設定する
	 * @param x 設定するx軸回転
	 * @param y 設定するy軸回転
	 * @param z 設定するz軸回転
	 * @param space 設定に使用する座標空間
	 */
	void SetEulerAngle(float x, float y, float z, Space space = Space::WORLD);

	/**
	 * @brief オブジェクトの回転をクォータニオンで設定する
	 * @param quaternion 設定する回転 (クォータニオン)
	 * @param space 設定に使用する座標空間
	 */
	void SetQuaternion(Quaternion quaternion, Space space = Space::WORLD);

	/**
	 * @brief 親Transformを設定する
	 * @param pParent 親ゲームオブジェクトへのポインタ
	 */
	void SetParent(GameObject* pParent);

	/**
	 * @brief 親Transformを設定する
	 * @param pParent 親Transformへのポインタ
	 */
	void SetParent(Transform* pParent);

	/**
	 * @brief 親Transformを取得する
	 * @return 親Transformへのポインタ
	 */
	Transform* GetParent() const;

	/**
	 * @brief ルートTransformを取得する
	 * @return 最上位のTransformへのポインタ
	 */
	Transform* GetRoot();

	/**
	 * @brief このTransformの子Transformを取得する
	 * @return 子Transformへのポインタ配列
	 */
	std::vector<Transform*> GetChildren() const;

	/**
	 * @brief オブジェクトを移動させる
	 * @param translation オブジェクトの移動量
	 * @param space 回転に使用する座標空間
	 */
	void Translate(Vector3 translation, Space space = Space::WORLD);

	/**
	 * @brief オブジェクトを移動させる
	 * @param x オブジェクトのx移動量
	 * @param y オブジェクトのy移動量
	 * @param z オブジェクトのz移動量
	 * @param space 回転に使用する座標空間
	 */
	void Translate(float x, float y, float z, Space space = Space::WORLD);

	/**
	 * @brief オブジェクトを回転させる
	 * @param euler 回転量 (オイラー)
	 * @param space 回転に使用する座標空間
	 */
	void Rotate(Vector3 euler, Space space = Space::WORLD);

	/**
	 * @brief オブジェクトを回転させる
	 * @param x x軸回転量
	 * @param y y軸回転量
	 * @param z z軸回転量
	 * @param space 回転に使用する座標空間
	 */
	void Rotate(float x, float y, float z, Space space = Space::WORLD);

	/**
	 * @brief ワールド変換行列を取得する
	 * @return このTransformのワールド変換行列
	 */
	DirectX::XMMATRIX GetWorldMatrix() const;

private:
	// Transformクラスでは隠す
	using Component::SetEnabled;
	using Component::Destroy;

	/// オブジェクトのローカル座標
	Vector3 m_localPosition;

	/// オブジェクトのローカルスケール
	Vector3 m_localScale;

	/// オブジェクトのローカルクォータニオン
	Quaternion m_localQuaternion;

	/// 親Transformへのポインタ
	Transform* m_pParent;

	/// 子Transformへのポインタ
	std::vector<Transform*> m_pChildren;

	/**
	 * @brief 子Transformから自身を切り離す
	 * @param child 切り離す子Transformコンポーネントへのポインタ
	 */
	void DetachChild(Transform* child);
};