#pragma once
#include <GameFrame/Component.h>
#include <Effeksser/Effekseer.h>
#include "Effeksser/EffekseerRendererDX11.h"
#include <DXGameFrame.h>

class EffeksserDebug :
    public Component
{
private:
    //Effekseerのコアインスタンス
    Effekseer::ManagerRef m_EfkManager = nullptr;
    EffekseerRendererDX11::RendererRef m_EfkRenderer = nullptr;

    //シングルトンパターン: 外部からの直接生成・コピーを禁止
    EffeksserDebug(const EffeksserDebug&) = delete;
    EffeksserDebug& operator=(const EffeksserDebug&) = delete;

    void SetupEffekseerModules(ID3D11Device* device, ID3D11DeviceContext* context);
public:
    EffeksserDebug();


    //更新: 毎フレーム呼び出す
    void Update()override;

    //描画開始: カメラと投影行列の設定
    void BeginDraw();

    //描画: エフェクトの描画処理を実行
    void Draw();

    //描画終了
    void EndDraw();

    //解放: アプリケーション終了時に呼び出す
    void Uninit();

    // 外部（GameSceneなど）からエフェクトの再生に使うマネージャを取得
    ::Effekseer::ManagerRef GetManager() const { return m_EfkManager; }
   
 ;
};

