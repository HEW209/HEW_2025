// DirectionalLight.cpp
#include <Component/DirectionalLight.h>
#include <GameFrame/Transform.h>

DirectionalLight* DirectionalLight::s_pMainLight = nullptr;

DirectionalLight::DirectionalLight() :
	m_lightColor(1.0f, 1.0f, 1.0f, 1.0f),
	m_ambientColor(0.5f, 0.5f, 0.5f, 0.5f),
	m_lightIntensity(1.0f)
{
	if (s_pMainLight == nullptr)
	{
		s_pMainLight = this;
	}
}

DirectionalLight::~DirectionalLight()
{
	if (s_pMainLight == this)
	{
		s_pMainLight = nullptr;
	}
}

void DirectionalLight::SetMain()
{
	s_pMainLight = this;
}

void DirectionalLight::SetLightColor(Color color)
{
	m_lightColor = color;
}

LightConstantBuffer DirectionalLight::GetLightCB()
{
	// ライト定数バッファ用データを作成
	LightConstantBuffer lightCB = {};
	Vector3 lightDir = GetTransform()->GetQuaternion() * Vector3::forward;
	lightCB.lightDir = { lightDir.x, lightDir.y, lightDir.z };
	lightCB.lightIntensity = m_lightIntensity;
	lightCB.lightColor = { m_lightColor.r, m_lightColor.g, m_lightColor.b };
	lightCB.ambientColor = { m_ambientColor.r, m_ambientColor.g, m_ambientColor.b };
	return lightCB;
}

void DirectionalLight::SetAmbientColor(Color color)
{
	m_ambientColor = color;
}

void DirectionalLight::SetLightIntensity(float intensity)
{
	m_lightIntensity = intensity;
}

DirectionalLight* DirectionalLight::GetMain()
{
	return s_pMainLight;
}
