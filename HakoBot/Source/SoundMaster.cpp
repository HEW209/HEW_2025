#include "SoundMaster.h"

SoundMaster::SoundMaster()
	: pXAudio2(nullptr)
	, pMasteringVoice(nullptr)
{
}

bool SoundMaster::Init()
{
	HRESULT result;

	// XAudio2‚Ì‰Šú‰»
	result = XAudio2Create(&pXAudio2);
	if (FAILED(result))
	{
		return false;
	}

	// MasteringVoiceì¬
	result = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundMaster::Uninit()
{
	pMasteringVoice->DestroyVoice();
	pXAudio2->Release();
	CoUninitialize();
}
