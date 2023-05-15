#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

#include "VulkanInstance.h"
#include "Surface.h"

struct QueueFamily
{
	VkQueue queue;
	std::optional<uint32_t> index;

	bool isComplete();
};

class LogicalDevice
{
public:
	LogicalDevice();
	void init(VkPhysicalDevice _physicalDevice, Surface& surface);

	~LogicalDevice();
	void cleanup();

	static VkPhysicalDevice findSuitablePhysicalDevice(VulkanInstance& instance, Surface& surface);
	static std::vector<VkPhysicalDevice> getPhysicalDevices(VulkanInstance& instance);
	static bool isPhysicalDeviceSuitable(VkPhysicalDevice device, Surface& surface);

	static std::vector<const char*> getRequiredExtensions();
	static std::vector<VkExtensionProperties> getSupportedExtensions(VkPhysicalDevice device);
	static bool isExtensionsSupported(VkPhysicalDevice device, std::vector<const char*> extensions);

private:
	VkDevice handle;
	VkPhysicalDevice physicalDevice;
	QueueFamily graphicsFamily, presentFamily;

	static std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device);
	static QueueFamily findGraphicsFamily(VkPhysicalDevice device);
	static QueueFamily findPresentFamily(VkPhysicalDevice device, Surface& surface);

	//std::vector<VkDeviceQueueCreateInfo> getQueueCreateInfos(float* priorities);

	template<typename... Params>
	void getQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& createInfos,
		uint32_t queueFamilyIndex, uint32_t queueCount, float* priorities,
		Params... params);

	void getQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& createInfos);
};

template<typename... Params>
void LogicalDevice::getQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& createInfos,
	uint32_t queueFamilyIndex, uint32_t queueCount, float* priorities,
	Params... params)
{
	VkDeviceQueueCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	createInfo.queueFamilyIndex = queueFamilyIndex;
	createInfo.queueCount = queueCount;
	createInfo.pQueuePriorities = priorities;
	createInfos.push_back(createInfo);

	getQueueCreateInfos(createInfos, params...);
}