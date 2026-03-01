#include "LevelSerializer.h"
#include <fstream>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

bool LevelSerializer::SaveBlockTemplate(const std::string& path, const BlockTemplateData& data) {
    json j;
    j["name"] = data.name;
    j["modelPath"] = data.modelPath;
    j["blocks"] = json::array();
    for (const auto& b : data.blocks) {
        j["blocks"].push_back({ b.x, b.y, b.z });
    }

    fs::path fsPath(path);
    if (fsPath.has_parent_path() && !fs::exists(fsPath.parent_path())) {
        fs::create_directories(fsPath.parent_path());
    }

    std::ofstream o(path);
    if (!o.is_open()) return false;
    o << j.dump(4) << std::endl;
    return true;
}

bool LevelSerializer::LoadBlockTemplate(const std::string& path, BlockTemplateData& outData) {
    if (!fs::exists(path)) return false;
    std::ifstream i(path);
    if (!i.is_open()) return false;

    json j;
    try { i >> j; }
    catch (...) { return false; }

    outData.name = j.value("name", "Untitled");
    outData.modelPath = j.value("modelPath", "");
    outData.blocks.clear();
    if (j.contains("blocks") && j["blocks"].is_array()) {
        for (const auto& b : j["blocks"]) {
            if (b.size() >= 3) {
                outData.blocks.push_back(Vec3Int{ b[0], b[1], b[2] });
            }
        }
    }
    return true;
}

bool LevelSerializer::SaveLevelData(const std::string& path, const LevelData& levelData) {
    json j;
    j["gridSize"] = { levelData.gridSize.x, levelData.gridSize.y, levelData.gridSize.z };
    j["inventory"] = levelData.inventoryBlockFiles;

    j["targets"] = json::object();
    j["targets"]["x"] = SerializeShape(levelData.targetShapes[0]);
    j["targets"]["y"] = SerializeShape(levelData.targetShapes[1]);
    j["targets"]["z"] = SerializeShape(levelData.targetShapes[2]);

    fs::path fsPath(path);
    if (fsPath.has_parent_path() && !fs::exists(fsPath.parent_path())) {
        fs::create_directories(fsPath.parent_path());
    }

    std::ofstream o(path);
    if (!o.is_open()) return false;
    o << j.dump(4) << std::endl;
    return true;
}

bool LevelSerializer::LoadLevelData(const std::string& path, LevelData& outLevelData) {
    if (!fs::exists(path)) return false;
    std::ifstream i(path);
    if (!i.is_open()) return false;

    json j;
    try { i >> j; }
    catch (...) { return false; }

    if (j.contains("gridSize")) {
        auto& s = j["gridSize"];
        outLevelData.gridSize = { s[0], s[1], s[2] };
    }

    if (j.contains("inventory")) {
        outLevelData.inventoryBlockFiles = j["inventory"].get<std::vector<std::string>>();
    }

    if (j.contains("targets")) {
        DeserializeShape(j["targets"]["x"], outLevelData.targetShapes[0]);
        DeserializeShape(j["targets"]["y"], outLevelData.targetShapes[1]);
        DeserializeShape(j["targets"]["z"], outLevelData.targetShapes[2]);
    }
    return true;
}

bool LevelSerializer::SaveAnswerData(const std::string& path, const AnswerData& answerData) {
    using json = nlohmann::json;
    json j;

    j["gridSize"] = { answerData.gridSize.x, answerData.gridSize.y, answerData.gridSize.z };

    j["targets"] = json::object();
    j["targets"]["x"] = SerializeShape(answerData.targetShapes[0]);
    j["targets"]["y"] = SerializeShape(answerData.targetShapes[1]);
    j["targets"]["z"] = SerializeShape(answerData.targetShapes[2]);

    j["placedBlocks"] = json::array();
    for (const auto& block : answerData.placedBlocks) {
        json jBlock;
        jBlock["modelPath"] = block.modelPath;

        jBlock["position"] = {
            {"x", block.position.x},
            {"y", block.position.y},
            {"z", block.position.z}
        };

        jBlock["rotation"] = {
            {"x", block.rotation.x},
            {"y", block.rotation.y},
            {"z", block.rotation.z},
            {"w", block.rotation.w}
        };

        jBlock["localBlocks"] = json::array();
        for (const auto& localPos : block.blockSet.blocks) {
            jBlock["localBlocks"].push_back({ localPos.x, localPos.y, localPos.z });
        }

        j["placedBlocks"].push_back(jBlock);
    }

    fs::path fsPath(path);
    if (fsPath.has_parent_path() && !fs::exists(fsPath.parent_path())) {
        fs::create_directories(fsPath.parent_path());
    }

    std::ofstream o(path);
    if (!o.is_open()) return false;
    o << j.dump(4) << std::endl;
    return true;
}

json LevelSerializer::SerializeShape(const ShapeType& shape) {
    json jShape = json::array();
    size_t w = shape.GetSize(0);
    size_t h = shape.GetSize(1);

    for (size_t y = 0; y < h; ++y) {
        json row = json::array();
        for (size_t x = 0; x < w; ++x) {
            row.push_back((bool)shape(x, y));
        }
        jShape.push_back(row);
    }
    return jShape;
}

void LevelSerializer::DeserializeShape(const json& j, ShapeType& outShape) {
    if (!j.is_array() || j.empty()) return;

    size_t h = j.size();
    size_t w = j[0].size();

    outShape.Resize((int)w, (int)h, false);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            if (x < j[y].size()) {
                outShape((int)x, (int)y) = j[y][x].get<bool>();
            }
        }
    }
}