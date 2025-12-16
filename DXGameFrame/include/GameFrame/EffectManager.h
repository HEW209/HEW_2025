#pragma once

#include <Effekseer/Effekseer.h>
#include "Effekseer/EffekseerRendererDX11.h"

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

class EffectManager {
public:
	void Init();
public:
	//ƒVƒ“ƒOƒ‹ƒgƒ“
	static EffectManager& Instance();

	void BeginDraw();
	void EndDraw();

	void SetupEffekseerModules();

	Effekseer::ManagerRef GetEffectManager() { return m_effectManager; }
	EffekseerRendererDX11::RendererRef GetEffectRenderer() { return m_effectRendererRef; }
private:
	Effekseer::ManagerRef m_effectManager;
	EffekseerRendererDX11::RendererRef m_effectRendererRef;
};

