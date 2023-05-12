#include "Surface.h"

#include "DebugMessenger.h"

Surface::Surface() :
	handle(nullptr),
	instanceHandle(nullptr)
{
}

void Surface::init(VulkanInstance& instance, Window& window)
{
	// TODO test for nullptr instanceHandle
	instanceHandle = instance.getHandle();
	VkResult result = glfwCreateWindowSurface(instanceHandle, window.getHandle(), nullptr, &handle); VK_CHECK(result);
}

Surface::~Surface()
{
	cleanup();
}

void Surface::cleanup()
{
	if (instanceHandle && handle) {
		vkDestroySurfaceKHR(instanceHandle, handle, nullptr);
		instanceHandle = nullptr;
		handle = nullptr;
	}
}

VkSurfaceCapabilitiesKHR Surface::getCapabilities(VkPhysicalDevice device)
{
	VkSurfaceCapabilitiesKHR capabilities;
	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, handle, &capabilities); VK_CHECK(result);
	return capabilities;
}

std::vector<VkSurfaceFormatKHR> Surface::getFormats(VkPhysicalDevice device)
{
	uint32_t count = 0;
	VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, handle, &count, nullptr); VK_CHECK(result);
	std::vector<VkSurfaceFormatKHR> formats(count);
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(device handle, &count, formats.data()); VK_CHECK(result);
	return formats;
}

std::vector<VkPresentModeKHR> Surface::getPresentModes(VkPhysicalDevice device)
{
	uint32_t count = 0;
	VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, handle, &count, nullptr); VK_CHECK(result);
	std::vector<VkPresentModeKHR> presentModes(count);
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, handle, &count, presentModes.data()); VK_CHECK(result);
	return presentModes;
}