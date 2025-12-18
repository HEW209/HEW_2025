// Collider.cpp

#include <Component/Collider.h>
#include <GameFrame/ColliderSystem.h>
#include <GameFrame/GameObject.h>

Collider::Collider() :Component::Component(), OnCollisionEnter(nullptr), IsStatic(true)
{
	// collider‚ð‚Ü‚Æ‚ß‚é”z—ñ‚É’Ç‰Á
	ColliderSystem::Instance().Register(this);

	m_scale = { 1.0f,1.0f,1.0f };

#ifdef _DEBUG
	renderer = nullptr;
#endif
}

Collider::~Collider()
{
	// collider‚ð‚Ü‚Æ‚ß‚é”z—ñ‚©‚çÁ‹Ž
	ColliderSystem::Instance().Unregister(this);
}

#ifdef _DEBUG
void Collider::Awake()
{
	renderer = GetGameObject()->AddComponent<MeshRenderer>();
}
#endif

