// Shader.cpp
#include <DirectX/Shader.h>
#include <DirectX/Debug.h>

void Shader::Load(const std::string& filePath)
{
	if (FAILED(LoadFromCSOFile(filePath)))
	{
		Debug::ErrorMessage(filePath + "の読み込みに失敗しました。\nプログラムを終了します。");
		exit(0);
	}
}