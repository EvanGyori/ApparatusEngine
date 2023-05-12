#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "VulkanInstance.h"

class LogicalDevice
{
public:
	LogicalDevice();
	void init(VkPhysicalDevice _physicalDevice);

	~LogicalDevice();
	void cleanup();

	static VkPhysicalDevice findSuitablePhysicalDevice(VulkanInstance& instance);
	static std::vector<VkPhysicalDevice> getPhysicalDevices(VulkanInstance& instance);
	static bool isPhysicalDeviceSuitable(VkPhysicalDevice device, Surface& surface);

	static std::vector<const char*> getRequiredExtensions();
	static std::vector<VkExtensionProperties> getSupportedExtensions(VkPhysicalDevice device);
	static bool isExtensionsSupported(VkPhysicalDevice device, std::vector<const char*> extensions);

private:
	VkDevice handle;
	VkPhysicalDevice physicalDevice;
};