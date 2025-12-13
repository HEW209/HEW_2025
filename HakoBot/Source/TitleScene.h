// タイトルシーン

#pragma once
#include <DXGameFrame.h>

class TitleScene : public Scene {
public:
    TitleScene() {};
    ~TitleScene() override = default;

    void Init() override;
};
