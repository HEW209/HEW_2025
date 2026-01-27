#pragma once

#include <DXGameFrame.h>

class TitleStage : public Component
{
public:
	TitleStage();
	virtual ~TitleStage() = default;

	void Awake() override;
	void Update() override;

private:
	struct Block
	{
		const char* filePath;
		Vector3 offset;
		Vector3 angle;
	};

	ObjPtr<MeshRenderer> m_pConveyors;
	ObjPtr<MeshRenderer> m_block;

	float m_time;

	void SetRandomBlock();
}; 