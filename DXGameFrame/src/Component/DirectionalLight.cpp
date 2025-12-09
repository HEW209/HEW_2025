// DirectionalLight.cpp
#include <Component/DirectionalLight.h>
#include <GameFrame/Transform.h>

DirectionalLight* DirectionalLight::s_pMainLight = nullptr;

DirectionalLight::DirectionalLight() :
	m_lightColor(1.0f, 1.0f, 1.0f, 1.0f),
	m_ambientColor(0.5f, 0.5f, 0.5f, 0.5f)
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

ConstantBuffer::Light DirectionalLight::GetLightData()
{
	ConstantBuffer::Light setting = {};
	Vector3 lightDir = GetTransform()->GetQuaternion() * Vector3::forward;
	setting.lightDir = { lightDir.x, lightDir.y, lightDir.z };
	setting.lightColor = { m_lightColor.r, m_lightColor.g, m_lightColor.b };
	setting.ambientColor = { m_ambientColor.r,m_ambientColor.g,m_ambientColor.b };
	return setting;
}
