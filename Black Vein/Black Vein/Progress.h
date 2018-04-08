/*

										/////////////////////////////////////////////////////
										////////LETS CREATE A VULKAN ENGINE FOR FUN//////////
										/////////////////////////////////////////////////////


												/////////////////////////////////
												//////THE JOURNEY RECORD/////////
												/////////////////////////////////




// Day 1:
 ==> refrences: vulkan tutorial on: https://vulkan.lunarg.com/doc/sdk/1.1.70.1/windows/tutorial/html/01-init_instance.html
 ==> created a vulkan instance

 ==> vulkan instance is basically a connection between the application and the vulkan library
 ==> creating the instance is the first step in building a vulkan application
 ==> creating vulkan objects involves creating structs that have info related to the object
	i.e: vkCreateInstance needs "VkInstanceCreateInfo" and "VkApplicationInfo" structs, each filled with neccessary info
==> we have to load vulkan functions from using vulkan loader, it's done using mainly "vkGetInstanceProcAddr" and "vkGetDeviceProcAddr"
==> the vulkan loader functions themselves have to be loaded from the dll using the regular dll loading function "LoadLibrary"



// Day 2:

 ==> refrences: vulkan tutorial on: https://vulkan.lunarg.com/doc/sdk/1.1.70.1/windows/tutorial/html/
 ==> got a list of physical devices and choose the first one as a default (my pc only has 1 anyway)
 ==> got a list of queue families and chose the one the can do graphics operations "VK_QUEUE_GRAPHICS_BIT"
 ==> created a logical device that "vkDevice" that will be responsible for directing graphics commands to the hardware
 ==> added a function that gives an error message and exit the program whenever an error occures and used it instead of "cout"

 ==> created a command buffer pool to allocate command buffers from it
 ==> each queue family can have 1 command buffer pool
 ==> we will use command buffers to record our orders for the GPU to do certain operations, i.e: draw, change line width
 
 ==> created a surface and connected it to the window
 ==> create a swapchain that we will use to get the images on the window
 ==> creating a swapchain involves many steps and most of them are just to fill-out the swapchain info structure
 ==> the steps are:
 ===> /// To create the swapchain we need:
 ===> ///		1- Surface
 ===> ///		2- Graphics queue index
 ===> ///		3- Present queue index
 ===> ///		4- Pixel format
 ===> ///		5- Fill out swapchain info structure:
 ===> ///			=> Surface capabilities
 ===> ///			=> Present modes
 ===> ///			=> Swapchain extents
 ===> ///			=> Minimum number of images in swapchain
 ===> ///			=> PreTransform ????
 ===> ///			=> Composite Alpha flags
 ===> ///		6- Create Swapchain
 ===> ///		7- Save Image and image views to use later on



// Day 3

 ==> created a depth buffer
 ==> depth buffer is made by:
		1- creating an image
		2- allocating memory and binding it to the image
			- while we are allocating the memory we need to know its type
			- used a function from the samples (util.cpp) that gets the memory type -Still don't understand it-
		3- creating an image view
*/