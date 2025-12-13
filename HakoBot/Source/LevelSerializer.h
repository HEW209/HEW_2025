#pragma once
#include <string>
#include "EditorTypes.h"
#include "nlohmann/json.hpp"

class LevelSerializer {
public:
    static bool SaveBlockTemplate(const std::string& path, const BlockTemplateData& data);
    static bool LoadBlockTemplate(const std::string& path, BlockTemplateData& outData);

    static bool SaveLevelData(const std::string& path, const LevelData& levelData);
    static bool LoadLevelData(const std::string& path, LevelData& outLevelData);

private:
    static nlohmann::json SerializeShape(const ShapeType& shape);
    static void DeserializeShape(const nlohmann::json& j, ShapeType& outShape);
};