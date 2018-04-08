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
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;  // NOTE(KAI): done (DAY 2)
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;  // NOTE(KAI): done (DAY 2)
	PFN_vkCreateDevice vkCreateDevice;  // NOTE(KAI): done (DAY 2)
	PFN_vkCreateCommandPool vkCreateCommandPool; // NOTE(KAI): done (DAY 2)
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers; // NOTE(KAI): done (DAY 2)
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
	PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
	PFN_vkCreateImageView vkCreateImageView;

	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
};

struct SwapChainBuffer 
{
	VkImage Image;
	VkImageView View;
};

struct VK_Data
{
	VkInstance Instance;	
	std::vector<VkPhysicalDevice> PhysicalDevices;

	uint32_t QueueFamilyPropertiesCount;
	uint32_t GraphicsQueueFamilyIndex; 
	uint32_t PresentQueueFamilyIndex; 
	std::vector<VkQueueFamilyProperties> QueueFamilyProperties; 

	VkDevice Device; 

	VkCommandPool CommandPool; 
	VkCommandBuffer CommandBuffer; 

	std::vector<const char*> InstanceExtensionNames; 
	std::vector<const char*> DeviceExtensionNames;

	HWND Window;

	VkSurfaceKHR Surface;
	VkFormat Format;

	uint32_t Width;
	uint32_t Height;

	VkSwapchainKHR SwapChain;
	uint32_t SwapChainImageCount;
	std::vector<SwapChainBuffer> Buffers;
};

static VK_Func Vulkan_Functions;
static VK_Data Vulkan_Data; 

void ExitOnError(const char *msg) 
{
	MessageBox(NULL, msg, NULL, MB_ICONERROR);
	std::cout << msg;

	exit(EXIT_FAILURE);
} 

PFN_vkVoidFunction GetFunctionPointer(VkInstance instance, char *name)	
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

///*NOTE(KAI)*///
/// Create a vulkan instance needs:
/// 1- Filling-out the Application info struct
/// 2- Filling-out the Instance info struct
/// 3- Filling-out the Instance info struct may need using extension name or layer names
/// 4- Till now we just used extension names for Surface Extensions
VkResult CreateVulkanInstance() 
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

	Vulkan_Data.InstanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	Vulkan_Data.InstanceExtensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	
	instanceInfo.enabledExtensionCount = Vulkan_Data.InstanceExtensionNames.size();
	instanceInfo.ppEnabledExtensionNames = Vulkan_Data.InstanceExtensionNames.data();

	result = Vulkan_Functions.vkCreateInstance(&instanceInfo, NULL, &Vulkan_Data.Instance);

	return result;
}

///*NOTE(KAI)*///
/// 1- Get physical devices
/// 2- Get queue family(s)
/// 3- Add device extensions for surfaces (or any other extension we may want)
/// 3- Get Logical device
VkResult CreateVulkanDevice() 
{
	VkResult result;

	// NOTE(KAI): Getting a list of physical devices
	uint32_t deviceCount;

	result = Vulkan_Functions.vkEnumeratePhysicalDevices(Vulkan_Data.Instance, &deviceCount, NULL);

	if (result != VK_SUCCESS)
	{
		ExitOnError("Couldn't get the number of physical devices\n");

		return result;
	}

	Vulkan_Data.PhysicalDevices.resize(deviceCount);

	result = Vulkan_Functions.vkEnumeratePhysicalDevices(Vulkan_Data.Instance
														, &deviceCount
														, Vulkan_Data.PhysicalDevices.data());
	
	if (result != VK_SUCCESS)
	{
		ExitOnError("Couldn't get a list of physical devices\n");

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
			Vulkan_Data.GraphicsQueueFamilyIndex = i;
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

	Vulkan_Data.DeviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	deviceInfo.enabledExtensionCount = Vulkan_Data.DeviceExtensionNames.size();
	deviceInfo.ppEnabledExtensionNames = Vulkan_Data.DeviceExtensionNames.data();

	Vulkan_Functions.vkCreateDevice(Vulkan_Data.PhysicalDevices[0], &deviceInfo, NULL, &Vulkan_Data.Device);
	return result;
}

VkResult InitCommandBufferPool() 
{
	VkCommandPoolCreateInfo poolInfo = {};
	
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = Vulkan_Data.GraphicsQueueFamilyIndex;

	VkResult result = Vulkan_Functions.vkCreateCommandPool(Vulkan_Data.Device, &poolInfo, NULL, &Vulkan_Data.CommandPool);

	return result;
}

VkResult InitCommandBuffer() 
{
	VkCommandBufferAllocateInfo bufferInfo = {};
	
	bufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	bufferInfo.commandPool = Vulkan_Data.CommandPool;
	bufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	bufferInfo.commandBufferCount = 1;

	VkResult result = Vulkan_Functions.vkAllocateCommandBuffers(Vulkan_Data.Device, &bufferInfo, &Vulkan_Data.CommandBuffer);

	return result;
}

VkResult InitSurface() 
{
	VkWin32SurfaceCreateInfoKHR surfaceInfo = {};

	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.hwnd = Vulkan_Data.Window;

	VkResult result = Vulkan_Functions.vkCreateWin32SurfaceKHR(Vulkan_Data.Instance, &surfaceInfo, NULL, &Vulkan_Data.Surface);

	return result;
}

///*NOTE(KAI)*///
/// To create the swapchain we need:
///		1- Surface
///		2- Graphics queue index
///		3- Present queue index
///		4- Pixel format
///		5- Fill out swapchain info structure:
///			=> Surface capabilities
///			=> Present modes
///			=> Swapchain extents
///			=> Minimum number of images in swapchain
///			=> PreTransform ????
///			=> Composite Alpha flags
///		6- Create Swapchain
///		7- Save Image and image views to use later on
VkResult InitSwapChain()
{
	VkResult res;

	///*NOTE(KAI)*///
	/// Get the graphics and present queue family index
	/// check if there is a queue that supports both present and graphics, if not look for separate queues
	{
		VkBool32 *supportPresent = (VkBool32*)malloc(Vulkan_Data.QueueFamilyPropertiesCount * sizeof(VkBool32));

		for (size_t i = 0; i < Vulkan_Data.QueueFamilyPropertiesCount; i++)
		{
			Vulkan_Functions.vkGetPhysicalDeviceSurfaceSupportKHR(Vulkan_Data.PhysicalDevices[0], i, Vulkan_Data.Surface, &supportPresent[i]);
		}

		Vulkan_Data.GraphicsQueueFamilyIndex = UINT32_MAX;
		Vulkan_Data.PresentQueueFamilyIndex = UINT32_MAX;

		for (size_t i = 0; i < Vulkan_Data.QueueFamilyPropertiesCount; i++)
		{
			if ((Vulkan_Data.QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
			{
				if (Vulkan_Data.GraphicsQueueFamilyIndex == UINT32_MAX)
				{
					Vulkan_Data.GraphicsQueueFamilyIndex = i;
				}

				if (supportPresent[i] == VK_TRUE)
				{
					Vulkan_Data.GraphicsQueueFamilyIndex = i;
					Vulkan_Data.PresentQueueFamilyIndex = i;

					break;
				}
			}
		}

		if (Vulkan_Data.PresentQueueFamilyIndex == UINT32_MAX)
		{
			for (size_t i = 0; i < Vulkan_Data.QueueFamilyPropertiesCount; i++)
			{
				if (supportPresent[i] == VK_TRUE)
				{
					Vulkan_Data.PresentQueueFamilyIndex = i;

					break;
				}
			}
		}

		free(supportPresent);

		if (Vulkan_Data.GraphicsQueueFamilyIndex == UINT32_MAX || Vulkan_Data.PresentQueueFamilyIndex == UINT32_MAX)
		{
			ExitOnError("Couldn't find either a suitable graphics queue or present queue or a queue with both capabilities\n");

			return VkResult::VK_ERROR_VALIDATION_FAILED_EXT;
		}
	}

	///*NOTE(KAI)*///
	/// Get supported pixel formats
	/// choose one of them
	{
		uint32_t formatCount;
		res = Vulkan_Functions.vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan_Data.PhysicalDevices[0]
			, Vulkan_Data.Surface
			, &formatCount
			, NULL);

		if (res != VK_SUCCESS)
		{
			ExitOnError("Failed to get format count\n");

			return VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED;
		}

		VkSurfaceFormatKHR *formats = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
		res = Vulkan_Functions.vkGetPhysicalDeviceSurfaceFormatsKHR(Vulkan_Data.PhysicalDevices[0]
			, Vulkan_Data.Surface
			, &formatCount
			, formats);

		if (res != VK_SUCCESS)
		{
			ExitOnError("Failed to get format count\n");

			return VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED;
		}
		else if (formatCount < 1)
		{
			ExitOnError("Failed to get format count\n");

			return VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED;
		}

		if (formatCount == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		{
			Vulkan_Data.Format = VK_FORMAT_B8G8R8A8_UNORM;
		}
		else
		{
			Vulkan_Data.Format = formats[0].format;
		}

		free(formats);
	}
	
	///*NOTE(KAI)*///
	/// Get the data neccessary for the swapchain info struct
	/// Create the swapchain
	/// Get the images and image views
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		res = Vulkan_Functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
			Vulkan_Data.PhysicalDevices[0]
			, Vulkan_Data.Surface
			, &surfaceCapabilities);

		if (res != VK_SUCCESS)
		{
			ExitOnError("Failed to get surface capabilities\n");

			return VkResult::VK_INCOMPLETE;
		}

		///*NOTE(KAI)*///
		/// Get supported present modes
		/// present modes are how we get images from the swapchain
		/// for now we will choose the default one "VK_PRESENT_MODE_FIFO_KHR"
		/*
		uint32_t presentModeCount;
		res = Vulkan_Functions.vkGetPhysicalDeviceSurfacePresentModesKHR(
			Vulkan_Data.PhysicalDevices[0]
			, Vulkan_Data.Surface
			, &presentModeCount
			, NULL);

		if (res != VK_SUCCESS)
		{
			ExitOnError("Failed to get present modes count\n");

			return VkResult::VK_INCOMPLETE;
		}

		VkPresentModeKHR *presentModes = (VkPresentModeKHR*)malloc(presentModeCount * sizeof(VkPresentModeKHR));
		res = Vulkan_Functions.vkGetPhysicalDeviceSurfacePresentModesKHR(
		Vulkan_Data.PhysicalDevices[0]
		, Vulkan_Data.Surface
		, &presentModeCount
		, presentModes);

		if (res != VK_SUCCESS)
		{
		ExitOnError("Failed to get present modes\n");

		return VkResult::VK_INCOMPLETE;
		}
		*/

		VkExtent2D swapchainExtent;

		if (surfaceCapabilities.currentExtent.width == 0xFFFFFFFF)
		{
			swapchainExtent.width = Vulkan_Data.Width;
			swapchainExtent.height = Vulkan_Data.Height;

			if (swapchainExtent.width < surfaceCapabilities.currentExtent.width)
			{
				swapchainExtent.width = surfaceCapabilities.currentExtent.width;
			}
			else if (swapchainExtent.width > surfaceCapabilities.currentExtent.width)
			{
				swapchainExtent.width = surfaceCapabilities.currentExtent.width;
			}

			if (swapchainExtent.height < surfaceCapabilities.currentExtent.height)
			{
				swapchainExtent.height = surfaceCapabilities.currentExtent.height;
			}
			else if (swapchainExtent.height > surfaceCapabilities.currentExtent.height)
			{
				swapchainExtent.height = surfaceCapabilities.currentExtent.height;
			}
		}
		else
		{
			swapchainExtent = surfaceCapabilities.currentExtent;
		}

		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

		uint32_t desiredNumberOfSwapChainImages = surfaceCapabilities.minImageCount;

		VkSurfaceTransformFlagBitsKHR preTransform;

		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = surfaceCapabilities.currentTransform;
		}

		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] =
		{
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
		};

		for (size_t i = 0; i < sizeof(compositeAlphaFlags); i++)
		{
			if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i])
			{
				compositeAlpha = compositeAlphaFlags[i];

				break;
			}
		}

		VkSwapchainCreateInfoKHR swapInfo = {};

		swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapInfo.surface = Vulkan_Data.Surface;
		swapInfo.minImageCount = desiredNumberOfSwapChainImages;
		swapInfo.imageFormat = Vulkan_Data.Format;
		swapInfo.imageExtent.width = swapchainExtent.width;
		swapInfo.imageExtent.height = swapchainExtent.height;
		swapInfo.preTransform = preTransform;
		swapInfo.compositeAlpha = compositeAlpha;
		swapInfo.imageArrayLayers = 1;
		swapInfo.presentMode = swapchainPresentMode;
		swapInfo.oldSwapchain = VK_NULL_HANDLE;
		swapInfo.clipped = true;
		swapInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapInfo.queueFamilyIndexCount = 0;
		swapInfo.pQueueFamilyIndices = NULL;

		uint32_t queueFamilyIndices[2] =
		{
			Vulkan_Data.GraphicsQueueFamilyIndex,
			Vulkan_Data.PresentQueueFamilyIndex
		};

		if (queueFamilyIndices[0] != queueFamilyIndices[1])
		{
			swapInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapInfo.queueFamilyIndexCount = 2;
			swapInfo.pQueueFamilyIndices = queueFamilyIndices;
		}

		res = Vulkan_Functions.vkCreateSwapchainKHR(Vulkan_Data.Device, &swapInfo, NULL, &Vulkan_Data.SwapChain);

		if (res != VK_SUCCESS)
		{
			ExitOnError("Failed to create swapchain\n");

			return VkResult::VK_INCOMPLETE;
		}


		res = Vulkan_Functions.vkGetSwapchainImagesKHR(Vulkan_Data.Device, Vulkan_Data.SwapChain, &Vulkan_Data.SwapChainImageCount, NULL);

		if (res != VK_SUCCESS)
		{
			ExitOnError("Failed to get swapchain image count\n");

			return VkResult::VK_INCOMPLETE;
		}

		VkImage *images = (VkImage*)malloc(Vulkan_Data.SwapChainImageCount * sizeof(VkImage));

		res = Vulkan_Functions.vkGetSwapchainImagesKHR(Vulkan_Data.Device, Vulkan_Data.SwapChain, &Vulkan_Data.SwapChainImageCount, images);

		if (res != VK_SUCCESS)
		{
			ExitOnError("Failed to get swapchain images\n");

			return VkResult::VK_INCOMPLETE;
		}

		Vulkan_Data.Buffers.resize(Vulkan_Data.SwapChainImageCount);

		for (size_t i = 0; i < Vulkan_Data.SwapChainImageCount; i++)
		{
			Vulkan_Data.Buffers[i].Image = images[i];
		}

		free(images);

		for (size_t i = 0; i < Vulkan_Data.SwapChainImageCount; i++)
		{
			VkImageViewCreateInfo imageInfo = {};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageInfo.image = Vulkan_Data.Buffers[i].Image;
			imageInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageInfo.format = Vulkan_Data.Format;
			imageInfo.components.r = VK_COMPONENT_SWIZZLE_R;
			imageInfo.components.g = VK_COMPONENT_SWIZZLE_G;
			imageInfo.components.b = VK_COMPONENT_SWIZZLE_B;
			imageInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			imageInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageInfo.subresourceRange.baseMipLevel = 0;
			imageInfo.subresourceRange.levelCount = 1;
			imageInfo.subresourceRange.baseArrayLayer = 0;
			imageInfo.subresourceRange.layerCount = 1;

			res = Vulkan_Functions.vkCreateImageView(Vulkan_Data.Device, &imageInfo, NULL, &Vulkan_Data.Buffers[i].View);

			if (res != VK_SUCCESS)
			{
				ExitOnError("Failed to get swapchain view\n");

				return VkResult::VK_INCOMPLETE;
			}
		}
	}	

	return res;
}

int InitVulkan()
{
	HMODULE vkLibrary = LoadLibrary("vulkan-1.dll");

	if (vkLibrary)
	{
		Vulkan_Functions.vkGetInstanceProcAddr = 
			(PFN_vkGetInstanceProcAddr)GetProcAddress(vkLibrary, "vkGetInstanceProcAddr"); 
		Vulkan_Functions.vkGetDeviceProcAddr = 
			(PFN_vkGetDeviceProcAddr)GetProcAddress(vkLibrary, "vkGetDeviceProcAddr"); 
	}

	if (!Vulkan_Functions.vkGetInstanceProcAddr || !Vulkan_Functions.vkGetDeviceProcAddr)
	{
		return NULL;
	}

	Vulkan_Functions.vkCreateInstance = (PFN_vkCreateInstance)GetFunctionPointer(NULL, "vkCreateInstance");

	if (CreateVulkanInstance() != VK_SUCCESS)
	{
		ExitOnError("Failed to create vulkan instance\n");
		
		return NULL;
	}

	Vulkan_Functions.vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)GetFunctionPointer(Vulkan_Data.Instance, "vkEnumeratePhysicalDevices");
	Vulkan_Functions.vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)GetFunctionPointer(Vulkan_Data.Instance, "vkGetPhysicalDeviceQueueFamilyProperties");
	Vulkan_Functions.vkCreateDevice = (PFN_vkCreateDevice)GetFunctionPointer(Vulkan_Data.Instance, "vkCreateDevice");
	Vulkan_Functions.vkCreateCommandPool = (PFN_vkCreateCommandPool)GetFunctionPointer(Vulkan_Data.Instance, "vkCreateCommandPool");
	Vulkan_Functions.vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)GetFunctionPointer(Vulkan_Data.Instance, "vkAllocateCommandBuffers");
	Vulkan_Functions.vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)GetFunctionPointer(Vulkan_Data.Instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
	Vulkan_Functions.vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)GetFunctionPointer(Vulkan_Data.Instance, "vkCreateWin32SurfaceKHR");
	Vulkan_Functions.vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)GetFunctionPointer(Vulkan_Data.Instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	Vulkan_Functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)GetFunctionPointer(Vulkan_Data.Instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	Vulkan_Functions.vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)GetFunctionPointer(Vulkan_Data.Instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	Vulkan_Functions.vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)GetFunctionPointer(Vulkan_Data.Instance, "vkCreateSwapchainKHR");
	Vulkan_Functions.vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)GetFunctionPointer(Vulkan_Data.Instance, "vkGetSwapchainImagesKHR");
	Vulkan_Functions.vkCreateImageView = (PFN_vkCreateImageView)GetFunctionPointer(Vulkan_Data.Instance, "vkCreateImageView");
	
	if (CreateVulkanDevice() != VK_SUCCESS)
	{
		ExitOnError("Failed to create vulkan physical device\n");
		
		return NULL;
	}

	if (InitCommandBufferPool() != VK_SUCCESS)
	{
		ExitOnError("Failed to create command buffer pooln\n");

		return NULL;
	}

	if (InitCommandBuffer() != VK_SUCCESS)
	{
		ExitOnError("Failed to create command buffer\n");

		return NULL;
	}

	if (InitSurface() != VK_SUCCESS)
	{
		ExitOnError("Failed to create surface\n");

		return NULL;
	}

	if (InitSwapChain() != VK_SUCCESS)
	{
		ExitOnError("Failed to create swapchain \n");

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

	if (RegisterClass(&windowClass))
	{
		Vulkan_Data.Window = CreateWindowEx(NULL
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

		if (Vulkan_Data.Window)
		{
			if (!InitVulkan())
			{
				ExitOnError("Failed to initialize vulkan\n");

				return 0;
			}

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
			ExitOnError("Couldn't create Window\n");
		}
	}
	else
	{
		ExitOnError("Couldn't register window class\n");
	}

	system("pause");

	return 0;
}

int main()
{
	WinMain(0, 0, 0, 0);

	return 0;
}