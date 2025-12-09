// Renderer.cpp
#include <Component/Renderer.h>
#include <GameFrame/RenderSystem.h>

Renderer::Renderer():
	m_isTransparent(false)
{
	RenderSystem::Instance().Register(this);
}

Renderer::~Renderer()
{
	RenderSystem::Instance().Unregister(this);
}

void Renderer::SetTransparent(bool transparent)
{
	m_isTransparent = transparent;
}

bool Renderer::IsTransparent()
{
	return m_isTransparent;
}
