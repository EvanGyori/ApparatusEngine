#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "VulkanInstance.h"
#include "Window.h"

class Surface
{
public:
	Surface();
	void init(VulkanInstance& instance, Window& window);

	~Surface();
	void cleanup();

	VkSurfaceCapabilitiesKHR getCapabilities(VkPhysicalDevice device);
	std::vector<VkSurfaceFormatKHR> getFormats(VkPhysicalDevice device);
	std::vector<VkPresentModeKHR> getPresentModes(VkPhysicalDevice device);

	VkBool32 supportsQueueFamily(VkPhysicalDevice device, uint32_t queueFamilyIndex);

private:
	VkInstance instanceHandle;
	VkSurfaceKHR handle;
};