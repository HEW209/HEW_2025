//Shader.cpp
#include <DirectX/Shader.h>
#include <DirectX/Debug.h>

void Shader::Load(const std::string& filePath)
{
	if (FAILED(LoadFromCSOFile(filePath)))
	{
		Debug::ErrorMessage(filePath + "ÇÃì«Ç›çûÇ›Ç…é∏îsÇµÇ‹ÇµÇΩÅB");
		exit(0);
	}
}