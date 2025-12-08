#pragma once
#include <xaudio2.h>
#include "SoundData.h"

class SoundPlayer
{
public:
	SoundPlayer();

	bool PlayWave(WaveData* outData, bool loop);
	void StopWave();

	void SetVolume(float volume);		// 0.0f ～ 1.0f
	void SetPitch(float pitchRatio);	// 1.0 = 原音, 2.0 = 1オクターブ高,　0.5 = 1オクターブ低

private:
	IXAudio2SourceVoice* pSourceVoice;
};