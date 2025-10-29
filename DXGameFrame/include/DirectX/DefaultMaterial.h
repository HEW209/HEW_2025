/******************************************************************//**
 * @file   DefaultMaterial.h
 * @brief  基本3D描画マテリアル
 * 
 * @author 石田怜
 * @date   2025/10/27
 *********************************************************************/
#pragma once

#include <DirectX/Material.h>

/**
 * @brief 基本3D描画マテリアル
 */
class DefaultMaterial : public Material
{
public:
	DefaultMaterial();

	void Bind() override;
};
