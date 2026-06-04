//TimeUI.h
#pragma once
#include <DXGameFrame.h>


class TimeUI : public Component
{
public:
	TimeUI();

	void Awake() override;			
	void Update() override;			

	enum class TextureType
	{
		GAME,
		RESULT
	};

	SpriteRenderer* m_digitRenderer[6];			//各桁の描画用
	SpriteRenderer* m_colonRenderer[2];			//コロン描画用

	void SetTextureType(TextureType textureType);

private:
	void SetDigitUV();					//UV座標を設定
};

