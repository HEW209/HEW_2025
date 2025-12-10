#pragma once
#include <vector>
#include <string>
#include "Vec.h"
#include "GameTypes.h" // ShapeTypeのために追加

// ブロックの配置データ（1つの塊）
struct BlockTemplateData {
    std::string name;
    std::vector<Vec3Int> blocks;
    std::string modelPath;
};

// レベル全体のデータ
struct LevelData {
    Vec3Int gridSize = { 5, 5, 5 };
    std::vector<std::string> inventoryBlockFiles;

    // ターゲット形状 (X軸方向, Y軸方向, Z軸方向)
    ShapeType targetShapes[3];
};