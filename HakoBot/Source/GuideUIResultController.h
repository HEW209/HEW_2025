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
	};

	//メニュー用
	int m_menuX = 0;
	int m_menuY = 0;
	struct MenuCell
	{
		float uvX;
		float uvY;
		float posX;
		float posY;
	};
	MenuCell menuTable[2][2] =
	{
		{   
			{ 0.0f, 0.0f, -5.0f,  2.8f }, 
			{ 0.5f, 0.0f,  5.0f,  2.8f }, 
		},
		{   
			{ 0.0f, 0.5f, -5.0f, -2.83f }, 
			{ 0.5f, 0.5f,  5.0f, -2.83f }, 
		}
	};

	ClosePhase m_closePhase;			//現在のメニュー閉じアニメーションの状態
	Vector3 m_closeStartScale;			//メニュー閉じアニメーションが始まるまえののスケール
	float m_closeValue;					//メニュー閉じアニメーションイージング用
};

