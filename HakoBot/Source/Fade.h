// Fade.h
#pragma once
#include <DXGameFrame.h>

class Fade : public Component
{
public:
	Fade();
	~Fade();

	void Awake() override;
	void Update() override;

	void SetFadeSpeed(float fadeSpeed);

private:
	SpriteRenderer* m_renderer;
	float m_fadeSpeed;

	enum class IconType
	{
		SIMPLE,
		SMILE,
		ANGRY,
		KIRAKIRA,
		RELAX,
		COUNT
	};
	void SetMaskIcon(IconType iconType);

public:
	static bool IsActive();
	static void SetFadeRatio(float ratio);

	static void StartIrisOut();
	static void StartIrisIn();
	static void StartIconIrisOut(bool maskIconChange = true);
	static void StartIconIrisIn(bool maskIconChange = false);

private:
	static Fade* s_main;

	enum FadeMode
	{
		SIMPLE_IRIS,
		ICON_IRIS
	};
	static FadeMode s_fadeMode;		// フェードモード
	static bool s_isFade;			// フェード状態フラグ
	static bool s_isActive;			// 動作中フラグ
	static float s_fadeRatio;		// フェード割合
	static int s_frameCount;

	static IconType s_maskIcon;

	static void StartFadeOut();
	static void StartFadeIn();
};
