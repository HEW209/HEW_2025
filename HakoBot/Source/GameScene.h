#pragma once
#include <DXGameFrame.h>
#include <string>
#include "EditorTypes.h"

class GameScene : public Scene {
public:
    GameScene(const std::string& levelName);
    ~GameScene() override = default;

    void Init() override;

private:
    void KeyBind();
    void CreateGridField();
    void CreateStageSet();
    void CreateUIObject();

    void CreateResultDebug();

    std::string m_levelName;
    LevelData m_levelData;
};