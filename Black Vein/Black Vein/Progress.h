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
 
 ==> created a uniform buffer to use to send the MVP matrix to the shaders
 ==> To create uniform buffer we need:
 ==> ///	1- The data we will put in the buffer (in this case the MVP matrix)
 ==> ///	2- The buffer
 ==> ///	3- The memory
 ==> ///	4- Put the data in the memory by: mapping the memory, copying the data to the memory then unmaping the memory
 ==> ///	5- bind the memory to the buffer

 ==> created a pipeline layout
 ==> Shaders explicitly use descriptors in the shading language, i.e: https://vulkan.lunarg.com/doc/sdk/1.1.70.1/windows/tutorial/html/08-init_pipeline_layout.html bottom of the page
 ==> ///	Shaders need descriptors to access uniforms
 ==> ///	Descriptors need to belong to descriptor set
 ==> ///	Descriptor sets need to belong to a descriptor set layout (which is used to describe a list of descriptor sets)
 ==> ///	Each descriptor set needs to have a layout binding (used to describe the descriptor set)
 ==> ///	Pipeline layout is used to describe a list of descriptor set layouts



// Day 4

 ==> allocated memory for the descriptor set
 ==> /// To allocate memory for descriptor set we need:
 ==> ///	1- Descriptor pool, one pool for each descriptor type (we only have one)
 ==> ///	2- Allocate memory for the descriptor
 ==> ///	3- Copy data from the uniform buffer to the descriptor

 ==> create render pass
 ==> render pass describes the render operation by specifying the attachments, subpasses and other dependencies
 ==> render pass has at least one subpass
 ==> example of attachment include: 
		 - color attachment which is the image aquired from the swapchain
		 - depth/stencil attachment which is the depth buffer we previously allocated
 ==> image attachments must be prepared before used
 ==> the preparation involves image layout transition from their initial state to an optimal state to use in the render pass
 ==> layout refers to how the texels are mapped from grid coordinates to offset in the memory
 ==> /// To create render pass we need:
 ==> ///		1- Create a semaphore for aquiring an image from the swapchain and prepare it for the render pass
					- a semaphore is normally used to hold back the rendering operation until the image is available
 ==> ///		2- Aquiring the image
 ==> ///		3- Prepare color and depth attachment by specifying the layout transition
 ==> ///		4- Describe the render subpass
 ==> ///		5- Create the render pass

 ==> created shader
 ==> /// To create a shader we need:
 ==> ///		1- The shader code in GLSL (here we have vertex and fragment shaders only)
 ==> ///		2- An array of unsigned int to hold the converted shader code
 ==> ///		3- Shader stages info
 ==> ///		4- The GLSL to SPIR-V conversion functions
 ==> ///		5- The shader modules
 ==> ///		6- We convert the GLSL to SPIR-V
 ==> ///		7- Fill-in the shader stages info
 ==> ///		8- Then we create the shader modules using the converted code and the shader stages
 ==> the conversion functions were taken from the vulkan samples
 ==> to use them we need both the include folders "glslang" and "SPIRV", both can be found in the vulkan SDK directory
 ==> we also need the following libraries:
	{
		glslangd.lib
		OGLCompilerd.lib
		SPIRVd.lib
		SPVRemapperd.lib
		HLSLd.lib
		OSDependentd.lib
		SPIRV-Toolsd.lib
		SPIRV-Tools-optd.lib
	}
	=> They can be found by generating a solution with cmake from the glslang folder in the vulkan SDK folder and then building it
	=> These are debug libraries


// Day 5
 
 ==> created frame buffer
 ==> frame buffer is a collection of memory attachment used in the render pass
 ==> here we put the color image and depth image in one frame buffer
 ==> because we get 2 images from the swapchain we created 2 frame buffers but both of them have the same depth image
 ==> /// To create frame buffers we need:
 ==> ///		1- Link the image views (color and depth here) to it via the frame buffer info struct 
 ==> ///		2- Create the frame buffer

*/