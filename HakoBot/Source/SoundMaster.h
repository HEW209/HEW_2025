#pragma once
#include <xaudio2.h>

class SoundMaster
{
private:
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasteringVoice;

	SoundMaster();

public:
	bool Init();
	void Uninit();

	// シングルトンインスタンス取得
	static SoundMaster& Instance() {
		static SoundMaster s_instance;
		return s_instance;
	}

	IXAudio2* GetXAudio2() { return pXAudio2; }
};