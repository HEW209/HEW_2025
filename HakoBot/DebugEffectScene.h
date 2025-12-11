#pragma once
#include <DXGameFrame.h>
#include "Effekseer/Effekseer.h"
#include "Effekseer/EffekseerRendererDX11.h"
#include"Effekseer/Effekseer.SIMD.h"
#include"Effekseer/Effekseer.Modules.h"
#include <Component/Camera.h> //カメラクラスを使うために必要



class DebugEffectScene :
    public Scene
{
public:
    DebugEffectScene();
    void Init();
    Effekseer::EffectRef m_loadedEffect;
};

