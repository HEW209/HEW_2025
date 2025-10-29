/*****************************************************************//**
 * @file   MeshRenderer.h
 * @brief  メッシュの描画を行う
 * 
 * @author 石田怜
 * @date   2025/10/25
 *********************************************************************/
#pragma once

#include <Component/Renderer.h>
#include <DirectX/Material.h>
#include <DirectX/Mesh.h>
#include <memory>

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	~MeshRenderer() = default;

	/**
	 * @brief 描画処理
	 */
	void Draw() override;

	/**
	 * @brief マテリアルを設定する
	 * @param pMaterial 設定するマテリアルへのポインタ
	 */
	void SetMaterial(std::shared_ptr<Material> pMaterial)
	{
		m_pMaterial = pMaterial;
	}

	/**
	 * @brief メッシュを設定する
	 * @param pMesh 設定するメッシュへのポインタ
	 */
	void SetMesh(std::shared_ptr<Mesh> pMesh)
	{
		m_pMesh = pMesh;
	}

private:
	/// マテリアルへのポインタ
	std::shared_ptr<Material> m_pMaterial;

	/// メッシュへのポインタ
	std::shared_ptr<Mesh> m_pMesh;
};
