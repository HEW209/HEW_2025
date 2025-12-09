#pragma once
#include <DXGameFrame.h>
#include "Effekseer/Effekseer.h"
#include "Effekseer/EffekseerRendererDX11.h"
#include"Effekseer/Effekseer.SIMD.h"
#include"Effekseer/Effekseer.Modules.h"
#include <Component/Camera.h> //カメラクラスを使うために必要
#include "EffekseerDebug.h"

#ifdef _DEBUG
#pragma comment(lib, "Effekseer/Effekseer.lib")
#else
#pragma comment(lib, "Effekseer/Effekseer.lib")
#endif
#ifdef _DEBUG
#pragma comment(lib, "Effekseer/EffekseerRendererDX11.lib")
#else
#pragma comment(lib, "Effekseer/EffekseerRendererDX11.lib")
#endif
class DebugEffectScene :
    public Scene
{
public:
    DebugEffectScene();
    void Init();
    Effekseer::EffectRef m_loadedEffect;
};

