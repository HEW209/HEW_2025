#pragma once
// GuideUIMenuController.h

#include <DXGameFrame.h>

class GuideUIMeneController : public Component
{
public:
	GuideUIMeneController();

	void Start() override;
	void Update() override;

	Vector3 m_defaultPosition;
	Vector3 m_defaultScale;

private:
	enum MenuState
	{
		DEFAULT,
		OPEN,
		SELECT,
		CLOSE,
		SCENE_CHANGE,
		MANUAL
	};

	MenuState m_menuState;
	float m_value1 = 0.0f;			//メニュー表示
	float m_value2 = 0.0f;			//メニュー非表示

	Vector3 m_targetScale;
	SpriteRenderer* m_buttons[4];
	SpriteRenderer* m_back;
	SpriteRenderer* m_frame;

	GameObject* m_manual;

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
	float m_bgScrollY;
	float m_bgScrollSpeed;

	void UpdateDefault();
	void UpdateOpen();
	void UpdateSelect();
	void UpdateClose();
	void UpdateChangeScene();
	void UpdateManual();

	void SelectEnter();
};