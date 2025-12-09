/******************************************************************//**
 * @file   DirectXInclude.h
 * @brief  DirectX関連機能の準備
 * 
 * @author 石田怜
 * @date   2025/11/21
 *********************************************************************/
#pragma once

#include <d3d11.h>								// Direct3Dの機能を使用
#pragma comment(lib, "d3d11.lib")				// Direct3Dの機能を使用
#pragma comment(lib, "dxgi.lib")				// グラフィックスデバイスやスワップチェインを使用
#include <DirectXMath.h>						// DirectXMath(数学ライブラリ)を使用
#include <d3dcompiler.h>						// シェーダーのコンパイルに必要
#pragma comment(lib, "d3dcompiler.lib")			// シェーダーのコンパイルに必要
#include <wrl/client.h>							// ComPtrを使用できるようにする
using Microsoft::WRL::ComPtr;					// ComPtrを使用できるようにする