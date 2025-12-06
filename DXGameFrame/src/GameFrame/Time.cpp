// Time.cpp
#include <GameFrame/Time.h>

float Time::s_deltaTime = 0.0f;
float Time::s_sceneTime = 0.0f;
float Time::s_totalTime = 0.0f;

void Time::Init()
{
	s_deltaTime = 0.0f;
	s_sceneTime = 0.0f;
	s_totalTime = 0.0f;
}

void Time::ResetSceneTime()
{
	s_sceneTime = 0.0f;
}

void Time::Update(float delta)
{
	s_deltaTime = delta;
	s_sceneTime += delta;
	s_totalTime += delta;
}

float Time::GetDeltaTime()
{
	return s_deltaTime;
}

float Time::GetSceneTime()
{
	return s_sceneTime;
}

float Time::GetTotalTime()
{
	return s_totalTime;
}