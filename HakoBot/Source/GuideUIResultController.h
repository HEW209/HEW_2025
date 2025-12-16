#pragma once
//GuideUIResultController.h

#include <DXGameFrame.h>

class GuideUIResultController : public Component
{
public:
	void Start() override;
	void Update() override;
	bool GetMenuBool();

	Vector3 m_defaultPosition;
	Vector3 m_defaultScale;
	bool m_menu;
private:
	float m_value1 = 0.0f;			//メニュー表示
	float m_value2 = 0.0f;			//メニュー非表示
	
	bool m_menu2;
	Vector3 m_targetScale;
	SpriteRenderer* m_rend;
	SpriteRenderer* m_rend2;

	//メニュー閉じるときのアニメーション管理
	enum class ClosePhase
	{
		None,
		Pop,		//少し拡大
		Shrink,		//縮小
		Delete		//消す
	};

	ClosePhase m_closePhase;
	Vector3 m_closeStartScale;
	float m_closeValue;
};

