#include <Windows.h>
#include <stdint.h>
#include <vulkan.h>
#include <iostream>
#include <vector>

struct VK_Func
{
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr; // NOTE(KAI): done (DAY 1)
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkCreateInstance vkCreateInstance; // NOTE(KAI): done (DAY 1)
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	//PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkCreateDevice vkCreateDevice;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
};

struct VK_Data
{
	VkInstance Instance;	// NOTE(KAI): done (DAY 1) 
	std::vector<VkPhysicalDevice> PhysicalDevices;

	uint32_t QueueFamilyPropertiesCount;
	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;

	VkPhysicalDeviceProperties DeviceProperties;
	VkDevice Device;
};

static VK_Func Vulkan_Functions;
static VK_Data Vulkan_Data;

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

	result = Vulkan_Functions.vkCreateInstance(&instanceInfo, NULL, &Vulkan_Data.Instance);

	return result;
}

///*NOTE(KA)*///
/// 1- Get physical devices
/// 2- Get queue family(s)
/// 3- Get Logical device
VkResult CreateVulkanDevice() // NOTE(KAI): done (DAY 2)
{
	VkResult result;

	// NOTE(KAI): Getting a list of physical devices
	uint32_t deviceCount;

	result = Vulkan_Functions.vkEnumeratePhysicalDevices(Vulkan_Data.Instance, &deviceCount, NULL);

	if (result != VK_SUCCESS)
	{
		std::cout << "Couldn't get the number of physical devices\n";

		return result;
	}

	Vulkan_Data.PhysicalDevices.resize(deviceCount);

	result = Vulkan_Functions.vkEnumeratePhysicalDevices(Vulkan_Data.Instance
														, &deviceCount
														, Vulkan_Data.PhysicalDevices.data());
	
	if (result != VK_SUCCESS)
	{
		std::cout << "Couldn't get a list of physical devices\n";

		return result;
	}

	//

	VkDeviceQueueCreateInfo queueInfo = {};

	// NOTE(KAI): Getting a list of queue family properties
	Vulkan_Functions.vkGetPhysicalDeviceQueueFamilyProperties(Vulkan_Data.PhysicalDevices[0]
															, &Vulkan_Data.QueueFamilyPropertiesCount
															, NULL);

	Vulkan_Data.QueueFamilyProperties.resize(Vulkan_Data.QueueFamilyPropertiesCount);

	Vulkan_Functions.vkGetPhysicalDeviceQueueFamilyProperties(Vulkan_Data.PhysicalDevices[0]
															, &Vulkan_Data.QueueFamilyPropertiesCount
															, Vulkan_Data.QueueFamilyProperties.data());

	// NOTE(KAI): Getting the queue family for the graphics operations
	for (size_t i = 0; i < Vulkan_Data.QueueFamilyProperties.size(); i++)
	{
		if (Vulkan_Data.QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueInfo.queueFamilyIndex = i;

			break;
		}
	}

	float queuePriorities[1] = { 1.0 };
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pQueuePriorities = queuePriorities;
	queueInfo.queueCount = 1;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &queueInfo;

	Vulkan_Functions.vkCreateDevice(Vulkan_Data.PhysicalDevices[0], &deviceInfo, NULL, &Vulkan_Data.Device);
	return result;
}

int InitVulkan()
{
	HMODULE vkLibrary = LoadLibrary("vulkan-1.dll");	// NOTE(KAI): done (DAY 1)

	if (vkLibrary)
	{
		Vulkan_Functions.vkGetInstanceProcAddr = 
			(PFN_vkGetInstanceProcAddr)GetProcAddress(vkLibrary, "vkGetInstanceProcAddr"); // NOTE(KAI): done (DAY 1)
		Vulkan_Functions.vkGetDeviceProcAddr = 
			(PFN_vkGetDeviceProcAddr)GetProcAddress(vkLibrary, "vkGetDeviceProcAddr"); // NOTE(KAI): done (DAY 2)
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

	// NOTE(KAI): done (DAY 2)
	Vulkan_Functions.vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)GetFunctionPointer(Vulkan_Data.Instance, "vkEnumeratePhysicalDevices");
	//Vulkan_Functions.vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)GetFunctionPointer(Vulkan_Data.Instance, "vkGetPhysicalDeviceProperties");
	Vulkan_Functions.vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)GetFunctionPointer(Vulkan_Data.Instance, "vkGetPhysicalDeviceQueueFamilyProperties");
	Vulkan_Functions.vkCreateDevice = (PFN_vkCreateDevice)GetFunctionPointer(Vulkan_Data.Instance, "vkCreateDevice");

	if (CreateVulkanDevice() != VK_SUCCESS)
	{
		std::cout << "Failed to create vulkan physical device\n";
		
		return NULL;
	}

	return 1;
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