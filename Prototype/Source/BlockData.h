/*********************************************************************
 * @file   BlockData.h
 * @brief  ブロックのデータを扱う構造体を定義
 *
 * @author 芝晃佑
 * @date   2025/11/15
 *********************************************************************/


#pragma once

#include <DXGameFrame.h>
#include <vector>

#include "Vec.h"


/**
 * @brief ブロックのデータ
 */
struct BlockSetData
{
	std::vector<Vec3Int> blocks;
};