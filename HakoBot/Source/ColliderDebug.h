/*********************************************************************
 * @file   ColliderDebug.h
 * @brief  当たり判定表示する Colliderのオフセット考慮してないので結構ガバガバです
 * 
 *
 * @author 三品怜
 * @date   2025/11/16
 *********************************************************************/

#pragma once

#include <DXGameFrame.h>

class ColliderDebug : public Component
{
public:

	ColliderDebug();

	void Update() override;
	bool m_IsDraw;
};
