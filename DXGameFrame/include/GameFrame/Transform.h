/*****************************************************************//**
 * @file   Transform.h
 * @brief  オブジェクトの座標・スケール・回転を表す
 * 
 * @author 石田怜
 * @date   2025/09/10
 *********************************************************************/
#pragma once

#include "Component.h"
#include <Utility/Vector3.h>
#include <Utility/Quaternion.h>

class Transform;

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
	Transform(GameObject* owner);
	~Transform();

	/// オブジェクトのローカル座標
	Vector3 m_position;

	/// オブジェクトのローカルスケール
	Vector3 m_scale;

	/**
	 * @brief オブジェクトの座標を取得する
	 * @param space 取得に使用する座標空間
	 * @return 現在のオブジェクトの座標
	 */
	Vector3 GetPosition(Space space = Space::WORLD);

	/**
	 * @brief オブジェクトのスケールを取得する
	 * @return 現在のオブジェクトのローカルスケール
	 */
	Vector3 GetScale();
	
	/**
	 * @brief オブジェクトの回転をオイラー角で取得する
	 * @param space 取得に使用する座標空間
	 * @return 現在のオブジェクトの回転 (オイラー角)
	 */
	Vector3 GetEulerAngle(Space space = Space::WORLD);

	/**
	 * @brief オブジェクトの回転をクォータニオンで取得する
	 * @param space 取得に使用する座標空間
	 * @return 現在のオブジェクトの回転 (クォータニオン)
	 */
	Quaternion GetQuaternion(Space space = Space::WORLD);

	/**
	 * @brief 座標を設定する
	 * @param position 設定するローカル座標
	 */
	void SetPosition(Vector3 position);

	/**
	 * @brief 座標を設定する
	 * @param x ローカルx座標
	 * @param y ローカルy座標
	 * @param z ローカルz座標
	 */
	void SetPosition(float x, float y, float z);

	/**
	 * @brief スケールを設定する
	 * @param scale 設定するローカルスケール
	 */
	void SetScale(Vector3 scale);

	/**
	 * @brief 座標を設定する
	 * @param x ローカルx座標
	 * @param y ローカルy座標
	 * @param z ローカルz座標
	 */
	void SetScale(float x, float y, float z);

	/**
	 * @brief ★オブジェクトの回転をオイラー角で設定する
	 * @param euler 設定する回転 (オイラー角)
	 */
	void SetEulerAngle(Vector3 euler);

	/**
	 * @brief オブジェクトの回転をオイラー角で設定する
	 * @param x x軸回転
	 * @param y y軸回転
	 * @param z z軸回転
	 */
	void SetEulerAngle(float x, float y, float z);

	/**
	 * @brief オブジェクトの回転をクォータニオンで設定する
	 * @param quaternion 設定する回転 (クォータニオン)
	 */
	void SetQuaternion(Quaternion quaternion);

	/**
	 * @brief 親Transformを設定する
	 * @param pParent 親Transformへのポインタ
	 */
	void SetParent(Transform* pParent);

	/**
	 * @brief 親Transformを取得する
	 * @return 親Transformへのポインタ
	 */
	Transform* GetParent();

	/**
	 * @brief このTransformの子Transformを取得する
	 * @return 子Transformへのポインタ配列
	 */
	std::vector<Transform*> GetChildren();

	/**
	 * @brief ワールド変換行列を取得する
	 * @return このTransformのワールド変換行列
	 */
	DirectX::XMMATRIX GetWorldMatrix();

	/**
	 * @brief ★オブジェクトを移動させる
	 * @param translation オブジェクトの移動量
	 */
	void Translate(Vector3 translation)
	{
		m_position += translation;
	}

	/**
	 * @brief ★オブジェクトを移動させる
	 * @param x オブジェクトのx移動量
	 * @param y オブジェクトのy移動量
	 * @param z オブジェクトのz移動量
	 */
	void Translate(float x, float y, float z)
	{
		m_position += Vector3(x, y, z);
	}

	/**
	 * @brief ★オブジェクトを回転させる
	 * @param euler 回転量 (オイラー)
	 */
	void Rotate(Vector3 euler)
	{
		m_euler += euler;
		m_quaternion = Quaternion::Euler(m_euler);
	}

	/**
	 * @brief ★オブジェクトを回転させる
	 * @param x x軸回転
	 * @param y y軸回転
	 * @param z z軸回転
	 */
	void Rotate(float x, float y, float z)
	{
		Rotate(Vector3(x, y, z));
	}

private:
	// Transformクラスでは隠す
	using Component::SetEnabled;
	using Component::Destroy;

	/// オブジェクトのローカルクォータニオン
	Quaternion m_quaternion;

	/// オブジェクトのローカルオイラー角
	Vector3 m_euler;

	/// 親Transformへのポインタ
	Transform* m_pParent;

	/// 子Transformへのポインタ
	std::vector<Transform*> m_pChildren;

	/**
	 * @brief 子Transform登録を削除する
	 * @param child 登録削除するTransformコンポーネントへのポインタ
	 */
	void DeleteChild(Transform* child);
};