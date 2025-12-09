#pragma once
#include <xaudio2.h>
#include <string>

struct WaveData
{
	WAVEFORMATEX m_wavFormat{};
	char* m_soundBuffer = nullptr;
	DWORD m_size = 0;

	~WaveData() { delete[] m_soundBuffer; }
};

class SoundData
{
public:
	SoundData();

	static bool LoadWave(const std::string& filePath, WaveData& outData);
};