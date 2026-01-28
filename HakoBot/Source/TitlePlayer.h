// TitlePlayer.h
#pragma once
#include <DXGameFrame.h>

class TitlePlayer : public Component
{
public:
	TitlePlayer();

	void Start() override;
	void Update() override;

private:
	struct Block
	{
		const char* filePath;
		Vector3 offset;
		Vector3 angle;
	};

	MeshRenderer* m_caterpillar;
	MeshRenderer* m_block;
	ObjPtr<GameObject> m_pBlockHolder;
	Vector2 m_uvOffset;
	bool m_isStart;

	void SetRandomBlock();
};
