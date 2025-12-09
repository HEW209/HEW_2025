#pragma once
#include <GameFrame/Component.h>
#include <Effekseer/Effekseer.h>
#include "Effekseer/EffekseerRendererDX11.h"
#include <DXGameFrame.h>
#include <d3d11.h>
class EffeksserDebug :
    public Component
{

private:
    //Effekseerのコアインスタンス
    Effekseer::ManagerRef m_EfkManager = nullptr;
    EffekseerRendererDX11::RendererRef m_EfkRenderer = nullptr;


public:
    EffeksserDebug();

    void SetupEffekseerModules();

    //更新: 毎フレーム呼び出す
    void Update()override;

    //描画開始: カメラと投影行列の設定
    void BeginDraw();

    //エフェクトの描画処理を実行
    void Draw();

};
