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
		float cameraScale = 1.0f;					// カメラスケール (平行投影で使用)
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
	 * @return ビュー行列
	 */
	DirectX::XMMATRIX GetViewMatrix();

	/**
	 * @brief このカメラからプロジェクション行列を作成する
	 * @return プロジェクション行列
	 */
	DirectX::XMMATRIX GetProjectionMatrix();

	/**
	 * @brief このカメラからシャドウ用のプロジェクション行列を作成する
	 * @param shadowDistance シャドウマップに映す距離
	 * @return プロジェクション行列
	 */
	DirectX::XMMATRIX GetShadowProjectionMatrix(float shadowDistance);

	/**
	 * @brief このカメラから平行投影プロジェクション行列を作成する
	 * @return プロジェクション行列
	 */
	DirectX::XMMATRIX GetOrthographicProjectionMatrix();

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
	static DirectX::XMMATRIX GetDefaultViewMatrix();

private:
	/// メインカメラへのポインタ
	static Camera* s_pMainCamera;
};