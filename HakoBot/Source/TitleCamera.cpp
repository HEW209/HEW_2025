#include "TitleCamera.h"

TitleCamera::TitleCamera()
{
    
    Camera::Config offsetConfig = this->GetConfig();
    //fovAngle = 10.0f;
    this->SetConfig(offsetConfig);
    auto transform = this->GetTransform();
    transform->SetPosition(10.0f, 6.5f, -10.0f,Space::WORLD);
    transform->SetEulerAngle(15.0f, -28.0f, 0.0f,Space::WORLD);
}

TitleCamera::~TitleCamera()
{
}
