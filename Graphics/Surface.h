#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "VulkanInstance.h"
#include "Window.h"

class Surface
{
public:
	/**
	 * @brief Default Constructor: Doesn't initilize surface, must call init
	 */
	Surface();

	/**
	 * @brief Creates a surface for the provided window
	 * 
	 * @param instance - the instance handle to create this surface under
	 * @param window - this surface refers to the provided window's surface
	 */
	void init(VulkanInstance& instance, Window& window);

	/**
	 * @brief Destructor: Calls cleanup() to free up memory
	 */
	~Surface();

	/**
	 * @brief Destroys the surface to free up memory
	 */
	void cleanup();

	/**
	 * @brief Returns handle to this surface.
	 * Limit uses of this function and use other functions when available.
	 * 
	 * @return surface handle
	 */
	VkSurfaceKHR getHandle();

	/**
	 * @brief Returns the surface's capabilities that are compatible with the specified device
	 * 
	 * @param device - the physical device to check for compatibility
	 * 
	 * @return compatible surface capabilities
	 */
	VkSurfaceCapabilitiesKHR getCapabilities(VkPhysicalDevice device);

	/**
	 * @brief Returns the surface's formats that are compatible with the specified device
	 * 
	 * @param device - the physical device to check for compatibility
	 * 
	 * @return vector of compatible surface formats
	 */
	std::vector<VkSurfaceFormatKHR> getFormats(VkPhysicalDevice device);

	/**
	 * @brief Returns the surface's present modes that are compatible with the specified device
	 * 
	 * @param device - the physical device to check for compatibility
	 * 
	 * @return vector of compatible present modes
	 */
	std::vector<VkPresentModeKHR> getPresentModes(VkPhysicalDevice device);

	/**
	 * @brief returns whether the specified queue family from the specified device supports
	 * presenting images to this surface
	 * 
	 * @param device - the physical device the queue family is on
	 * @param queueFamilyIndex - the index of the queue family to check for compatibility
	 * 
	 * @return VK_TRUE if presenting to this surface is supported. VK_FALSE otherwise.
	 */
	VkBool32 supportsQueueFamily(VkPhysicalDevice device, uint32_t queueFamilyIndex);

private:
	VkInstance instanceHandle;
	VkSurfaceKHR handle;
};