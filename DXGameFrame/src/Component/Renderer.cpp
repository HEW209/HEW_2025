// Renderer.cpp
#include <Component/Renderer.h>
#include <GameFrame/RenderSystem.h>

Renderer::Renderer():
	m_isTransparent(false),
	m_isGroupTransparent(false)
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
	m_isGroupTransparent &= transparent;
}

bool Renderer::IsTransparent()
{
	return m_isTransparent;
}

void Renderer::SetGroupTransparent(bool transparent)
{
	m_isGroupTransparent = transparent;
	m_isTransparent |= transparent;
}

bool Renderer::IsGroupTransparent()
{
	return m_isGroupTransparent;
}