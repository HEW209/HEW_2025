/*********************************************************************
 * @file   TitleExit.h
 * @brief  タイトルから他のシーンに遷移する
 *
 * @author 三品怜
 * @date   2025/12/10
 *********************************************************************/

#pragma once

#include <DXGameFrame.h>

class TitleExit :public Component
{
public:

	TitleExit() {};
	~TitleExit() = default;

	void Update()override;
};
