// Collider.cpp

#include <Component/Collider.h>
#include <System/ColliderSystem.h>

Collider::Collider():Component::Component(),onCollision(nullptr)
{
	// collider‚ğ‚Ü‚Æ‚ß‚é”z—ñ‚É’Ç‰Á
	ColliderSystem::Instance().Register(this);

	m_scale = { 1.0f,1.0f,1.0f };
}

Collider::~Collider()
{
	// collider‚ğ‚Ü‚Æ‚ß‚é”z—ñ‚©‚çÁ‹
	ColliderSystem::Instance().Unregister(this);
}

