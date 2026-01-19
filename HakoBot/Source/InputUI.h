// InputUI.h
#pragma once
#include <DXGameFrame.h>

class InputUI : public Component
{
public:
	InputUI();

	void Awake() override;
	void Update() override;

private:
	enum class Basic
	{
		MOTU,
		TAKASA,
		KAITEN,
		COUNT
	};

	enum class Special
	{
		BLOCK_GHOST,
		ROTATE_LOCK,
		COUNT
	};

	SpriteRenderer* m_menuButton;
	SpriteRenderer* m_menuText;
	SpriteRenderer* m_basicTexts[(UINT)Basic::COUNT];
	SpriteRenderer* m_basicButtons[(UINT)Basic::COUNT];
	SpriteRenderer* m_buttonLB;
	SpriteRenderer* m_specialTexts[(UINT)Special::COUNT];
	SpriteRenderer* m_specialButtons[(UINT)Special::COUNT];

	void SwitchButton();
	void PushScaling();
};
