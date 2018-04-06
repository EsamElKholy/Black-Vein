/*

										/////////////////////////////////////////////////////
										////////LETS CREATE A VULKAN ENGINE FOR FUN//////////
										/////////////////////////////////////////////////////


												/////////////////////////////////
												//////THE JOURNEY RECORD/////////
												/////////////////////////////////




// Day 1:
 ==> created a vulkan instance
 ==> refrences: vulkan tutorial on: https://vulkan.lunarg.com/doc/sdk/1.1.70.1/windows/tutorial/html/01-init_instance.html

 ==> vulkan instance is basically a connection between the application and the vulkan library
 ==> creating the instance is the first step in building a vulkan application
 ==> creating vulkan objects involves creating structs that have info related to the object
	i.e: vkCreateInstance needs "VkInstanceCreateInfo" and "VkApplicationInfo" structs, each filled with neccessary info
==> we have to load vulkan functions from using vulkan loader, it's done using mainly "vkGetInstanceProcAddr" and "vkGetDeviceProcAddr"
==> the vulkan loader functions themselves have to be loaded from the dll using the regular dll loading function "LoadLibrary"


*/