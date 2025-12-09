/*****************************************************************//**
 * @file   Common.h
 * @brief  汎用マクロなどを定義
 * 
 * @author 石田怜
 * @date   2025/10/18
 *********************************************************************/
#pragma once

#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

//クランプマクロ
#define CLAMP(v, minV, maxV)   ((v)<(minV)?(minV):((v)>(maxV)?(maxV):(v)))

//メモリ解放マクロ
#define SAFE_DELETE(p)			do{if(p){delete p; p = nullptr;}}while(0)
#define SAFE_DELETE_ARRAY(p)	do{if(p){delete[] p; p = nullptr;}}while(0)
#define SAFE_RELEASE(p)			do{if(p){p->Release(); p = nullptr;}}while(0)