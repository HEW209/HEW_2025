// Renderer.cpp
#include <Component/Renderer.h>
#include <System/RenderSystem.h>

Renderer::Renderer()
{
	RenderSystem::Instance().Register(this);
}

Renderer::~Renderer()
{
	RenderSystem::Instance().Unregister(this);
}