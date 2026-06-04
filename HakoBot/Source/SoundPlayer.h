#pragma once
#include <xaudio2.h>
#include "SoundData.h"

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	bool PlayWave(WaveData* outData, bool loop, float volume = 1.0f);
	void StopWave();

	void SetPitch(float pitchRatio);	// 1.0 = 原音, 2.0 = 1オクターブ高,　0.5 = 1オクターブ低

	bool IsPlaying() const;

private:
	IXAudio2SourceVoice* pSourceVoice;
};