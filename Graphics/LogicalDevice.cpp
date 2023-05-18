#include "LogicalDevice.h"

#include <cstring>

#include "DebugMessenger.h"

LogicalDevice::LogicalDevice() :
	handle(nullptr),
	physicalDevice(nullptr),
	graphicsFamily{},
	presentFamily{}
{
}

void LogicalDevice::init(VkPhysicalDevice _physicalDevice, Surface& surface)
{
	physicalDevice = _physicalDevice;
	if (physicalDevice == nullptr) {
		VK_CHECK(VK_ERROR_INCOMPATIBLE_DRIVER);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	graphicsFamily.index = findGraphicsFamily(physicalDevice);
	presentFamily.index = findPresentFamily(physicalDevice, surface);
	float priority = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	if (graphicsFamily.index.value() == presentFamily.index.value()) {
		getQueueCreateInfos(queueCreateInfos,
			graphicsFamily.index.value(), 1, &priority);
	} else {
		getQueueCreateInfos(queueCreateInfos,
			graphicsFamily.index.value(), 1, &priority,
			presentFamily.index.value(), 1, &priority);
	}
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkPhysicalDeviceFeatures features{};
	createInfo.pEnabledFeatures = &features;

	VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &handle); VK_CHECK(result);

	vkGetDeviceQueue(handle, graphicsFamily.index.value(), 0, &graphicsFamily.queue);
	vkGetDeviceQueue(handle, presentFamily.index.value(), 0, &presentFamily.queue);
}

LogicalDevice::~LogicalDevice()
{
	cleanup();
}

void LogicalDevice::cleanup()
{
	if (handle) {
		vkDestroyDevice(handle, nullptr);
		handle = nullptr;
	}
}

VkPhysicalDevice LogicalDevice::findSuitablePhysicalDevice(VulkanInstance& instance, Surface& surface)
{
	auto physicalDevices = getPhysicalDevices(instance);
	for (auto device : physicalDevices) {
		if (isPhysicalDeviceSuitable(device, surface)) {
			return device;
		}
	}

	return nullptr;
}

std::vector<VkPhysicalDevice> LogicalDevice::getPhysicalDevices(VulkanInstance& instance)
{
	uint32_t count = 0;
	vkEnumeratePhysicalDevices(instance.getHandle(), &count, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(count);
	vkEnumeratePhysicalDevices(instance.getHandle(), &count, physicalDevices.data());
	return physicalDevices;
}

bool LogicalDevice::isPhysicalDeviceSuitable(VkPhysicalDevice device, Surface& surface)
{
	auto extensions = getRequiredExtensions();
	if (!isExtensionsSupported(device, extensions)) {
		return false;
	}

	// Check if the surface and physicalDevice supports the swapchain details needed
	if (surface.getFormats(device).empty() || surface.getPresentModes(device).empty()) {
		return false;
	}

	// Check if the device supports the needed queues and that the present queue supports presenting to the specific surface
	std::optional<uint32_t> graphicsFamilyIndex = findGraphicsFamily(device);
	std::optional<uint32_t> presentFamilyIndex = findPresentFamily(device, surface);
	if (!graphicsFamilyIndex.has_value() || !presentFamilyIndex.has_value()) {
		return false;
	}

	return true;
}

std::vector<const char*> LogicalDevice::getRequiredExtensions()
{
	return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}

std::vector<VkExtensionProperties> LogicalDevice::getSupportedExtensions(VkPhysicalDevice device)
{
	uint32_t count = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
	std::vector<VkExtensionProperties> extensions(count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());
	return extensions;
}

bool LogicalDevice::isExtensionsSupported(VkPhysicalDevice device, std::vector<const char*> extensions)
{
	auto supportedExtensions = getSupportedExtensions(device);
	for (auto extension : extensions) {
		bool supported = false;
		for (auto supportedExtension : supportedExtensions) {
			if (std::strcmp(extension, supportedExtension.extensionName) == 0) {
				supported = true;
			}
		}

		if (!supported) return false;
	}

	return true;
}

std::vector<VkQueueFamilyProperties> LogicalDevice::getQueueFamilies(VkPhysicalDevice device)
{
	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());
	return queueFamilies;
}

std::optional<uint32_t> LogicalDevice::findGraphicsFamily(VkPhysicalDevice device)
{
	std::optional<uint32_t> graphicsFamilyIndex{};

	auto queueFamilies = getQueueFamilies(device);
	for (int i = 0; i < queueFamilies.size(); i++) {
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamilyIndex = i;
			return graphicsFamilyIndex;
		}
	}

	// graphics queue family wasn't found
	return graphicsFamilyIndex;
}

std::optional<uint32_t> LogicalDevice::findPresentFamily(VkPhysicalDevice device, Surface& surface)
{
	std::optional<uint32_t> presentFamilyIndex{};

	auto queueFamilies = getQueueFamilies(device);
	for (int i = 0; i < queueFamilies.size(); i++) {
		if (surface.supportsQueueFamily(device, i)) {
			presentFamilyIndex = i;
			return presentFamilyIndex;
		}
	}

	// present queue family wasn't found
	return presentFamilyIndex;
}

void LogicalDevice::getQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& createInfos)
{
}