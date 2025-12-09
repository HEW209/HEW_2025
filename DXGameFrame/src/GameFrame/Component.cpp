//Component.cpp
#include <GameFrame/Component.h>
#include <GameFrame/GameObject.h>

Component::Component():
	m_pGameObject(nullptr),
	m_pTransform(nullptr),
	m_enabled(true),
	m_destroyed(false),
	m_started(false)
{
}

void Component::SetGameObject(GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = pGameObject->GetTransform();
}
