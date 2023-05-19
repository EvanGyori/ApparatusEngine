#pragma once

#include "LogicalDevice.h"
#include "Surface.h"

class Swapchain
{
public:
	Swapchain();
	void init(LogicalDevice& device, Surface& surface, Window& window);

	~Swapchain();
	void cleanup();

private:
	VkDevice deviceHandle;
	VkSwapchainKHR handle;

	uint32_t pickMinImageCount(VkSurfaceCapabilitiesKHR capabilities);
	VkSurfaceFormatKHR pickFormat(std::vector<VkSurfaceFormatKHR> formats);
	VkExtent2D pickExtent(VkSurfaceCapabilitiesKHR capabilities, Window& window);
};