#pragma once

#include <Component/Renderer.h>
#include <Effekseer/Effekseer.h>
#include <GameFrame/Transform.h>

/**
* @brief DirectX::XMMATRIX を Effekseer::Matrix43 に変換します。
* * DirectX::XMMATRIX (行優先、4x4) を Effekseer::Matrix43 (列優先、3x4) に変換します。
* * @param dxMatrix 変換元の DirectX::XMMATRIX
* @return 変換された Effekseer::Matrix43
*/
Effekseer::Matrix43 ConvertXMMatrixToEffekseerMatrix43(const DirectX::XMMATRIX& dxMatrix);
Effekseer::Matrix44 ConvertXMMatrixToEffekseerMatrix44(const DirectX::XMMATRIX& dxMatrix);
class EffectRenderer : public Component
{
public:
	EffectRenderer();
	~EffectRenderer();
	void Draw();
	void Load(const std::string& fileName);
	void Play();
	void Stop();

private:
	Effekseer::EffectRef m_effect;
	Effekseer::Handle m_handle;

};
