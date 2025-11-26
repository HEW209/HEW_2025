/******************************************************************//**
 * @file   Debug.h
 * @brief  デバッグ用機能
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include <Windows.h>
#include <string>

/**
 * @brief デバッグ用機能
 */
class Debug
{
public:
	/**
	 * @brief エラーメッセージを表示する
	 * @param message 表示するメッセージ
	 */
	static void ErrorMessage(const std::string& message);
};