/*****************************************************************//**
 * @file   Camera.h
 * @brief  カメラ情報を扱う
 *
 * @author 石田怜
 * @date   2025/11/24
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>
#include <DirectX/DirectXInclude.h>
#include <Utility/Vector2.h>

/**
 * @brief カメラ情報を扱う
 */
class Camera : public Component
{
public:
	Camera();
	~Camera();

	/// カメラ設定
	struct Config
	{
		float fovAngle = 60.0f;						// 視野角
		Vector2 screenSize = { 1280.0f, 720.0f };	// 画面サイズ
		float nearZ = 0.1f;							// 最近距離
		float farZ = 1000.0f;						// 最遠距離
	};

	/**
	 * @brief カメラ設定を行う
	 * @param fov カメラ設定情報
	 */
	void SetConfig(Config config);

	/**
	 * @brief 現在のカメラ設定を取得する
	 * @return カメラ設定
	 */
	Config GetConfig();

	/**
	 * @brief このカメラをメインカメラに設定する
	 */
	void SetMain();

	/**
	 * @brief このカメラからビュー行列を作成する
	 * @return 転置されたビュー行列
	 */
	DirectX::XMFLOAT4X4 GetViewMatrix();

	/**
	 * @brief このカメラからプロジェクション行列を作成する
	 * @return 転置されたプロジェクション行列
	 */
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	/**
	 * @brief このカメラから平行投影プロジェクション行列を作成する
	 * @return 転置されたプロジェクション行列
	 */
	DirectX::XMFLOAT4X4 GetOrthographicProjectionMatrix();

private:
	/// カメラ設定
	Config m_config;

public:
	/**
	 * @brief メインカメラを取得する
	 * @return メインカメラへのポインタ
	 */
	static Camera* GetMain();

	/**
	 * @brief デフォルトビュー行列を取得する
	 * @return デフォルトビュー行列
	 */
	static DirectX::XMFLOAT4X4 GetDefaultViewMatrix();

private:
	/// メインカメラへのポインタ
	static Camera* s_pMainCamera;
};