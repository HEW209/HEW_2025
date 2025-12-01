/******************************************************************//**
 * @file   DXGameFrame.h
 * @brief  DXGameFrameの全機能を使用する
 * 
 * @author 石田怜
 * @date   2025/11/21
 *********************************************************************/
#pragma once

#ifdef _X86_
#ifdef _DEBUG
#pragma comment(lib, "DXGameFrame/lib/Win32/Debug/DXGameFrame.lib")
#else
#pragma comment(lib, "DXGameFrame/lib/Win32/Release/DXGameFrame.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "DXGameFrame/lib/x64/Debug/DXGameFrame.lib")
#else
#pragma comment(lib, "DXGameFrame/lib/x64/Release/DXGameFrame.lib")
#endif
#endif

#include "DirectX/DirectX.h"
#include "GameFrame/GameFrame.h"
#include "Utility/Utility.h"
#include "Component/DefaultComponent.h"
#include "../imgui/ImguiManager.h"
