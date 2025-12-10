#pragma once
#include <unordered_map>
#include "SoundData.h"
#include "SoundPlayer.h"

class SoundManager
{
public:
	// 読み込み
	static void Load();

	// BGM
	static void PlayBGM(const std::string& name, float volume = 1.0f, bool loop = true);
	static void StopBGM();

	// SE
	static void PlaySE(const std::string& name, float volume = 1.0f, bool loop = true);
	static void StopSE();

	// 全てのサウンド停止
	static void StopAll();

private:
	struct BGM
	{
		WaveData data;
	};

	struct SE
	{
		WaveData data;
	};

	static std::unordered_map<std::string, BGM> bgmMap;
	static std::unordered_map<std::string, SE> seMap;
	static SoundPlayer bgmPlayer;
	static SoundPlayer sePlayer;
};