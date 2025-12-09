#include "SoundData.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

SoundData::SoundData()
{
}

//--- Waveファイル読み込み
bool SoundData::LoadWave(const std::string& filePath, WaveData& outData)
{
	if (outData.m_soundBuffer)
	{
		delete[] outData.m_soundBuffer;
		outData.m_soundBuffer = nullptr;
	}

	HMMIO mmioHandle = nullptr;

	// チャンク情報
	MMCKINFO chunkInfo{};
	// RIFFチャンク用
	MMCKINFO riffChunkInfo{};

	// WAVファイルを開く
	mmioHandle = mmioOpenA(
		const_cast<LPSTR>(filePath.c_str()),
		nullptr,
		MMIO_READ
	);

	if (!mmioHandle) return false;

	// RIFFチャンクに侵入するためにfccTypeにWAVEを設定
	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFFチャンクに侵入
	if (mmioDescend(
		mmioHandle,
		&riffChunkInfo,
		nullptr,
		MMIO_FINDRIFF
	) != MMSYSERR_NOERROR)
	{
		// Riffチャンクに侵入失敗
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// 侵入先のチャンクを"fmt"として設定
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(
		mmioHandle,
		&chunkInfo,
		&riffChunkInfo,
		MMIO_FINDCHUNK
	) != MMSYSERR_NOERROR)
	{
		// fmtチャンクがない
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// fmtデータの読み込み
	DWORD readSize = mmioRead(
		mmioHandle,						// ハンドル
		(HPSTR)&outData.m_wavFormat,	// 読み込み用バッファ
		chunkInfo.cksize				// バッファサイズ
	);

	if (readSize != chunkInfo.cksize)
	{
		// 読み込みサイズが一致していない
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットチェック
	if (outData.m_wavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		// Waveフォーマットエラー
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	//	mmioAscend(mmioHandle, &chunkInfo, 0);

		// fmtチャンクを退出
	if (mmioAscend(mmioHandle, &chunkInfo, 0) != MMSYSERR_NOERROR)
	{
		// fmtチャンク退出失敗
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// dataチャンクに侵入
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(
		mmioHandle,
		&chunkInfo,
		&riffChunkInfo,
		MMIO_FINDCHUNK
	) != MMSYSERR_NOERROR)
	{
		// dataチャンク侵入失敗
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// サイズ保存
	outData.m_size = chunkInfo.cksize;

	// dataチャンク読み込み
	outData.m_soundBuffer = new char[chunkInfo.cksize];
	readSize =
		mmioRead(mmioHandle, (HPSTR)outData.m_soundBuffer, chunkInfo.cksize);

	if (readSize != chunkInfo.cksize)
	{
		// dataチャンク読み込み失敗
		mmioClose(mmioHandle, MMIO_FHOPEN);
		delete[] outData.m_soundBuffer;
		return false;
	}

	// ファイルを閉じる
	mmioClose(mmioHandle, MMIO_FHOPEN);

	return true;
}