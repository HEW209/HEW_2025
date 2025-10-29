/*****************************************************************//**
 * @file   Renderer.h
 * @brief  3D描画コンポーネントの基底クラス
 *
 * @author 石田怜
 * @date   2025/09/22
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>

 /**
  * @brief 3D描画コンポーネントの基底クラス
  */
class Renderer : public Component
{
public:
	Renderer();
	virtual ~Renderer();

	/**
	 * @brief 描画処理
	 */
	virtual void Draw() = 0;
};