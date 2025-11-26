#pragma once

#include <DXGameFrame.h>

class TransformGUI : public Component
{
public:
	TransformGUI();

	void Update() override;

	void SetName(const std::string& name)
	{
		m_name = name;
	}

private:
	std::string m_name;
};