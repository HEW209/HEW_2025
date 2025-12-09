/******************************************************************//**
 * @file   DXGameFrame.h
 * @brief  DXGameFrameの全機能を使用する
 * 
 * @author 石田怜
 * @date   2025/10/27
 *********************************************************************/
#pragma once

#ifdef _X86_
#ifdef _DEBUG
#pragma comment(lib, "Win32/Debug/DXGameFrame.lib")
#else
#pragma comment(lib, "Win32/Release/DXGameFrame.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "x64/Debug/DXGameFrame.lib")
#else
#pragma comment(lib, "x64/Release/DXGameFrame.lib")
#endif
#endif

#include "GameFrame/GameFrame.h"
#include "DirectX/DirectX.h"
#include "Component/DefaultComponent.h"
#include "Utility/Utility.h"
#include "System/System.h"