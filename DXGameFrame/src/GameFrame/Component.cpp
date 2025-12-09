// Component.cpp
#include <GameFrame/Component.h>
#include <GameFrame/GameObject.h>

Component::Component() :
	m_pGameObject(nullptr),
	m_pTransform(nullptr),
	m_enabled(true),
	m_destroyed(false),
	m_started(false)
{
}

GameObject* Component::GetGameObject() const
{
	return m_pGameObject;
}

Transform* Component::GetTransform() const
{
	return m_pTransform;
}

void Component::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool Component::IsEnabled() const
{
	return m_enabled;
}

bool Component::IsStarted() const
{
	return m_started;
}

void Component::Destroy()
{
	m_destroyed = true;
}

void Component::SetGameObject(GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = pGameObject->GetTransform();
}

void Component::SetStarted()
{
	m_started = true;
}

bool Component::IsDestroyed() const
{
	return m_destroyed;
}
