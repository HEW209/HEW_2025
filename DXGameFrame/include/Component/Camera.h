/*****************************************************************//**
 * @file   Camera.h
 * @brief  カメラ情報を扱う
 * 
 * @author 石田怜
 * @date   2025/10/25
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>

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
		float fovAngle = 60.0f;					//視野角
		float screenRatio = 16.0f / 9.0f;		//画面比率
		float nearZ = 0.1f;						//最近距離
		float farZ = 1000.0f;					//最遠距離
	};

	/**
	 * @brief カメラ設定を行う
	 * @param fov カメラ設定情報
	 */
	void SetConfig(Config config)
	{
		m_config = config;
	}

	/**
	 * @brief カメラ設定を取得する
	 * @return カメラ設定
	 */
	Config GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief このカメラをメインカメラに設定する
	 */
	void SetMain()
	{
		s_pMainCamera = this;
	}

private:
	/// カメラ設定
	Config m_config;

public:
	/**
	 * @brief メインカメラを取得する
	 * @return メインカメラへのポインタ
	 */
	static Camera* GetMain()
	{
		return s_pMainCamera;
	}

private:
	/// メインカメラへのポインタ
	static Camera* s_pMainCamera;
};