#pragma once
#include <DXGameFrame.h>
#include "Effeksser/Effekseer.h"
#include "Effeksser/EffekseerRendererDX11.h"
#include"Effeksser/Effekseer.SIMD.h"
#include"Effeksser/Effekseer.Modules.h"
#include <Component/Camera.h> //カメラクラスを使うために必要
#include "EffeksserDebug.h"

#ifdef _DEBUG
#pragma comment(lib, "Effeksser/Effekseer.lib")
#else
#pragma comment(lib, "Effeksser/Effekseer.lib")
#endif
#ifdef _DEBUG
#pragma comment(lib, "Effeksser/EffekseerRendererDX11.lib")
#else
#pragma comment(lib, "Effeksser/EffekseerRendererDX11.lib")
#endif
class DebugEffectScene :
    public Scene
{
public:
    DebugEffectScene();
    void Init();
    Effekseer::EffectRef m_loadedEffect;
};

