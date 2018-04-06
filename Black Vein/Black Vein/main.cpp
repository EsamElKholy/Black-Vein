#include <Windows.h>
#include <stdint.h>
#include <vulkan.h>
#include <iostream>

struct VK_Func
{
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr; // NOTE(KAI): done (DAY 1)
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkCreateInstance vkCreateInstance; // NOTE(KAI): done (DAY 1)
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkCreateDevice vkCreateDevice;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
};

static VkInstance Vulkan_Instance; // NOTE(KAI): done (DAY 1) 


static VkDevice Vulkan_Device;
static VK_Func Vulkan_Functions;
static VkPhysicalDevice Vulkan_PhysicalDevice;
static VkQueueFamilyProperties Vulkan_QueueFamilyProperties;
static VkPhysicalDeviceProperties Vulkan_DeviceProperties;


PFN_vkVoidFunction GetFunctionPointer(VkInstance instance, char *name)	// NOTE(KAI): done (DAY 1)
{
	PFN_vkVoidFunction result = 0;

	result = Vulkan_Functions.vkGetInstanceProcAddr(instance, name);

	return result;
}

PFN_vkVoidFunction GetDeviceRelatedFunctionPointer(VkDevice device, char *name)
{
	PFN_vkVoidFunction result = 0;

	result = Vulkan_Functions.vkGetDeviceProcAddr(device, name);

	return result;
}

VkResult CreateVulkanInstance() // NOTE(KAI): done (DAY 1)
{
	VkResult result;

	VkInstanceCreateInfo instanceInfo = {};

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan app test";
	appInfo.pEngineName = "Vulkan app test";
	appInfo.apiVersion = VK_API_VERSION_1_0;	

	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;

	result = Vulkan_Functions.vkCreateInstance(&instanceInfo, NULL, &Vulkan_Instance);

	return result;
}

VkResult CreateVulkanPhysicalDevice()
{
	VkResult result;

	uint32_t deviceCount;
	result = Vulkan_Functions.vkEnumeratePhysicalDevices(Vulkan_Instance, &deviceCount, &Vulkan_PhysicalDevice);

	if (result != VK_SUCCESS)
	{
		return result;
	}

	Vulkan_Functions.vkGetPhysicalDeviceProperties(Vulkan_PhysicalDevice, &Vulkan_DeviceProperties);

	uint32_t queueFamilyPropertyCount;
	Vulkan_Functions.vkGetPhysicalDeviceQueueFamilyProperties(Vulkan_PhysicalDevice, &queueFamilyPropertyCount, &Vulkan_QueueFamilyProperties);

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	Vulkan_Functions.vkCreateDevice(Vulkan_PhysicalDevice, &deviceInfo, NULL, &Vulkan_Device);
	return result;
}

int InitVulkan()
{
	HMODULE vkLibrary = LoadLibrary("vulkan-1.dll");	// NOTE(KAI): done (DAY 1)

	if (vkLibrary)
	{
		Vulkan_Functions.vkGetInstanceProcAddr = 
			(PFN_vkGetInstanceProcAddr)GetProcAddress(vkLibrary, "vkGetInstanceProcAddr"); // NOTE(KAI): done (DAY 1)
		Vulkan_Functions.vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)GetProcAddress(vkLibrary, "vkGetDeviceProcAddr");
	}

	if (!Vulkan_Functions.vkGetInstanceProcAddr || !Vulkan_Functions.vkGetDeviceProcAddr) // NOTE(KAI): done (DAY 1)
	{
		return NULL;
	}

	Vulkan_Functions.vkCreateInstance = (PFN_vkCreateInstance)GetFunctionPointer(NULL, "vkCreateInstance"); // NOTE(KAI): done (DAY 1)

	if (CreateVulkanInstance() != VK_SUCCESS) // NOTE(KAI): done (DAY 1)
	{
		std::cout << "Failed to create vulkan instance\n";
		return NULL;
	}

	Vulkan_Functions.vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)GetFunctionPointer(Vulkan_Instance, "vkEnumeratePhysicalDevices");
	Vulkan_Functions.vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)GetFunctionPointer(Vulkan_Instance, "vkGetPhysicalDeviceProperties");
	Vulkan_Functions.vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)GetFunctionPointer(Vulkan_Instance, "vkGetPhysicalDeviceProperties");
	Vulkan_Functions.vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)GetFunctionPointer(Vulkan_Instance, "vkGetPhysicalDeviceQueueFamilyProperties");
	Vulkan_Functions.vkCreateDevice = (PFN_vkCreateDevice)GetFunctionPointer(Vulkan_Instance, "vkCreateDevice");

	if (CreateVulkanPhysicalDevice() != VK_SUCCESS)
	{
		std::cout << "Failed to create vulkan physical device\n";
	}

	return NULL;
}

LRESULT CALLBACK WindowCallBack(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	result = DefWindowProc(window, message, wParam, lParam);

	return result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{
	WNDCLASS windowClass = {};

	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = "VulkanTest";
	windowClass.lpfnWndProc = WindowCallBack;

	bool isRunning = false;

	if (!InitVulkan())
	{
		std::cout << "Failed to initialize vulkan\n";

		system("pause");
		
		return 0;
	}

	if (RegisterClass(&windowClass))
	{
		HWND window = CreateWindowEx(NULL
			, windowClass.lpszClassName
			, "Vulkan Test"
			, WS_OVERLAPPEDWINDOW | WS_VISIBLE
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, NULL
			, NULL
			, hInstance
			, NULL);

		if (window)
		{
			isRunning = true;
			while (isRunning)
			{
				MSG message;

				while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
				{
					if (message.message == WM_QUIT)
					{
						isRunning = false;
					}

					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}
		else
		{
			std::cout << "Couldn't create Window\n";
		}
	}
	else
	{
		std::cout << "Couldn't register window class\n";
	}

	system("pause");

	return 0;
}

int main()
{
	WinMain(0, 0, 0, 0);

	return 0;
}