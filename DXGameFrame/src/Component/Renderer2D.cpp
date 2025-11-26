// Renderer2D.cpp
#include <Component/Renderer2D.h>
#include <GameFrame/RenderSystem.h>

Renderer2D::Renderer2D() :
	m_drawOrder(0),
	m_isUI(false)
{
	RenderSystem::Instance().Register2D(this);
}

Renderer2D::~Renderer2D()
{
	RenderSystem::Instance().Unregister2D(this);
}

int Renderer2D::GetOrder() const
{
	return m_drawOrder;
}

void Renderer2D::SetOrder(int order)
{
	m_drawOrder = order;
}

void Renderer2D::SetUI(bool isUI)
{
	m_isUI = isUI;
}

bool Renderer2D::IsUI()
{
	return m_isUI;
}
