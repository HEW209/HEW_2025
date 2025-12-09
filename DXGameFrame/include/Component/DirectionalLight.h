/******************************************************************//**
 * @file   DirectionalLight.h
 * @brief  指向性ライト情報を扱う
 * 
 * @author 石田怜
 * @date   2025/11/13
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>
#include <Utility/Color.h>
#include <DirectX/ConstantBuffer.h>

 /**
  * @brief 指向性ライト情報を扱う
  */
class DirectionalLight : public Component
{
public:
	DirectionalLight();
	~DirectionalLight();

	/**
	 * @brief このライトをメインライトに設定する
	 */
	void SetMain()
	{
		s_pMainLight = this;
	}

	/**
	 * @brief 光源色を設定する
	 * @param color 光源色
	 */
	void SetLightColor(Color color)
	{
		m_lightColor = color;
	}

	/**
	 * @brief ライト情報を取得する
	 * @return 定数バッファ用ライト情報
	 */
	ConstantBuffer::Light GetLightData();

	/**
	 * @brief 環境光を設定する
	 * @param color 環境光色
	 */
	void SetAmbientColor(Color color)
	{
		m_ambientColor = color;
	}

private:
	/// 光源カラー
	Color m_lightColor;

	/// 環境光
	Color m_ambientColor;

public:
	/**
	 * @brief メインライトを取得する
	 * @return メインライトへのポインタ
	 */
	static DirectionalLight* GetMain()
	{
		return s_pMainLight;
	}

private:
	/// メインライトへのポインタ
	static DirectionalLight* s_pMainLight;
};