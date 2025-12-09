//Debug.cpp
#include <DirectX/Debug.h>

void Debug::ErrorMessage(const std::string& message)
{
	MessageBox(NULL, message.c_str(), "ƒGƒ‰[", MB_OK | MB_ICONERROR);
}
