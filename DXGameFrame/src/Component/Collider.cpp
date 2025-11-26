// Collider.cpp

#include <Component/Collider.h>
#include <GameFrame/ColliderSystem.h>

Collider::Collider() :Component::Component(), OnCollisionEnter(nullptr), IsStatic(true)
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

