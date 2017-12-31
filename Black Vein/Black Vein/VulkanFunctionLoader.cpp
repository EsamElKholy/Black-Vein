#include "VulkanFunctionLoader.h"

namespace BlackVein
{
	bool LoadFunctionExportedFromVulkan(HMODULE & vulkan_library)
	{
#define EXPORTED_VULKAN_FUNCTION( name )									 \
		name = (PFN_##name)LoadFunction( vulkan_library, #name );			 \
		if( name == nullptr )												 \
		{																	 \
			std::cout << "Could not load exported Vulkan function named: "   \
			#name << std::endl;                                              \
			return false;                                                    \
		}

#include "ListOfVulkanFunctions.inl"

		return true;
	}
}

