// Camera.cpp
#include <Component/Camera.h>

Camera* Camera::s_pMainCamera = nullptr;

Camera::Camera()
{
	if (s_pMainCamera == nullptr)
	{
		s_pMainCamera = this;
	}
}

Camera::~Camera()
{
	if (s_pMainCamera == this)
	{
		s_pMainCamera = nullptr;
	}
}