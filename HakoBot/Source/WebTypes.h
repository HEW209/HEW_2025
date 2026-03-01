#pragma once
#include <vector>
#include <string>
#include "Vec.h"
#include "BlockData.h"
#include "GameTypes.h"

struct AnswerBlockData {
    std::string modelPath;
    Vector3 position;
    Quaternion rotation;
    BlockSetData blockSet;
};

struct AnswerData {
    Vec3Int gridSize;
    ShapeType targetShapes[3];
    std::vector<AnswerBlockData> placedBlocks;
};