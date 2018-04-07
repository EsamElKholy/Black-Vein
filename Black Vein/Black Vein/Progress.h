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
 
*/