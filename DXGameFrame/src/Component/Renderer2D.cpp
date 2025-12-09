// Renderer2D.cpp
#include <Component/Renderer2D.h>
#include <System/RenderSystem.h>

Renderer2D::Renderer2D()
	:m_drawOrder(0)
{
	RenderSystem::Instance().Register2D(this);
}

Renderer2D::~Renderer2D()
{
	RenderSystem::Instance().Unregister2D(this);
}