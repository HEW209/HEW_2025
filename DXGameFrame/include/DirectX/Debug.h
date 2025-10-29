/*****************************************************************//**
 * @file   Debug.h
 * @brief  デバッグ用機能を扱う
 * 
 * @author 石田怜
 * @date   2025/09/15
 *********************************************************************/
#pragma once

#include <Windows.h>
#include <string>

/**
 * @brief デバッグ用機能を扱う
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