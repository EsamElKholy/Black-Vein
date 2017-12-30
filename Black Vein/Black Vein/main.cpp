#include <iostream>
#include <Windows.h>

int main() 
{
	std::cout << "Let's begin \n";

	HMODULE vulkan_library = LoadLibrary("vulkan-1.dll");

	if (vulkan_library == nullptr)
	{
		std::cout << "failed to load vulkan library!! \n";
	}

	system("pause");

	return 0;
}