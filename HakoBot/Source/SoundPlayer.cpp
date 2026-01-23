#include "SoundPlayer.h"
#include "SoundMaster.h"

SoundPlayer::SoundPlayer()
	: pSourceVoice(nullptr)
{
}

SoundPlayer::~SoundPlayer()
{
	StopWave();
}

//--- Waveファイル再生
bool SoundPlayer::PlayWave(WaveData* outData, bool loop, float volume)
{
	StopWave(); // 既存のVoiceを破棄

	if (!outData) return false; // Waveファイル読み込み失敗

	// XAudio2本体を取得
	IXAudio2* xa2;
	xa2 = SoundMaster::Instance().GetXAudio2();
	if (!xa2) return false;

	//--- SourseVoiceの作成
	WAVEFORMATEX waveFormat{};

	// 波形フォーマットの設定
	memcpy(&waveFormat, &outData->m_wavFormat, sizeof(outData->m_wavFormat));

	// 1サンプル当たりのバッファサイズを算出
	waveFormat.wBitsPerSample =
		outData->m_wavFormat.nBlockAlign * 8 / outData->m_wavFormat.nChannels;

	// ソースボイスの作成
	HRESULT result =
		xa2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&waveFormat);
	if (FAILED(result)) return false; // 作成失敗


	//--- 波形データ(音データ本体)をソースボイスに渡す
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = (BYTE*)outData->m_soundBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = outData->m_size;

	// 三項演算子を用いてループするか否かの設定
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	pSourceVoice->SubmitSourceBuffer(&buffer);

	// 音量設定
	if (volume < 0.0f)volume = 0.0f;
	if (volume > 1.0f)volume = 1.0f;
	pSourceVoice->SetVolume(volume);

	// 音を鳴らす
	pSourceVoice->Start();


	return true;
}

//--- 再生停止
void SoundPlayer::StopWave()
{
	if (pSourceVoice)
	{
		pSourceVoice->Stop(0);
		pSourceVoice->FlushSourceBuffers(); // 再生キューをクリア

		// 破棄前にXAudio2がまだ存在しているかどうか確認
		if (SoundMaster::Instance().GetXAudio2())
		{
			pSourceVoice->DestroyVoice();
		}

		pSourceVoice = nullptr;
	}
}

void SoundPlayer::SetPitch(float pitchRatio)
{
	if (pSourceVoice)
	{
		if (pitchRatio < XAUDIO2_MIN_FREQ_RATIO)
		{
			pitchRatio = XAUDIO2_MIN_FREQ_RATIO;
		}
		if (pitchRatio > XAUDIO2_MAX_FREQ_RATIO)
		{
			pitchRatio = XAUDIO2_MAX_FREQ_RATIO;
		}
		pSourceVoice->SetFrequencyRatio(pitchRatio);
	}
}

bool SoundPlayer::IsPlaying() const
{
	if (pSourceVoice)
	{
		XAUDIO2_VOICE_STATE state{};
		pSourceVoice->GetState(&state);
		return (state.BuffersQueued > 0);
	}
	return false;
}
