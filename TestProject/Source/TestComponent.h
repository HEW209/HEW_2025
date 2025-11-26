#pragma once

#include <DXGameFrame.h>

class TestComponent : public Component
{
public:
	TestComponent();
	~TestComponent() = default;

	void Update() override;

private:
	enum class TransformMode
	{
		MOVE,
		ROTATE
	};

	TransformMode m_transMode;
	Space m_space;
};
