/*****************************************************************//**
 * @file   MatrixUtil.h
 * @brief  行列の作成を行う
 * 
 * @author 石田怜
 * @date   2025/10/25
 *********************************************************************/
#pragma once

#include <DirectX/DirectXInclude.h>
#include <GameFrame/GameObject.h>
#include <GameFrame/Transform.h>
#include <Component/Camera.h>

/**
 * @brief 行列の作成を行う
 */
class MatrixUtil
{
public:
	/**
	 * @brief Transformコンポーネントからワールド行列を求める
	 * @param pTransForm Transformコンポーネントへのポインタ
	 * @return 転置されたワールド行列
	 */
	static DirectX::XMFLOAT4X4 CreateWorldMatrix(Transform* pTransform);

	/**
	 * @brief Cameraコンポーネントからビュー行列を求める
	 * @param pCamera Cameraコンポーネントへのポインタ
	 * @return 転置されたビュー行列
	 */
	static DirectX::XMFLOAT4X4 CreateViewMatrix(Camera* pCamera);

	/**
	 * @brief Cameraコンポーネントからプロジェクション行列を求める
	 * @param pCamera Cameraコンポーネントへのポインタ
	 * @return 転置されたプロジェクション行列
	 */
	static DirectX::XMFLOAT4X4 CreateProjectionMatrix(Camera* pCamera);

private:
	MatrixUtil() = delete;
};
