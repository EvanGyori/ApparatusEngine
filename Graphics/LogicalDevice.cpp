#include "LogicalDevice.h"

#include <cstring>

#include "DebugMessenger.h"

bool QueueFamily::isComplete()
{
	return index.has_value();
}

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

	graphicsFamily = findGraphicsFamily(physicalDevice);
	presentFamily = findPresentFamily(physicalDevice, surface);
	float priority = 1.0f;
	auto queueCreateInfos = getQueueCreateInfos(&priority);
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkPhysicalDeviceFeatures features{};
	createInfo.pEnabledFeatures = &features;

	VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &handle); VK_CHECK(result);
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
	QueueFamily graphicsFamily = findGraphicsFamily(device);
	QueueFamily presentFamily = findPresentFamily(device, surface);
	if (!graphicsFamily.isComplete() || !presentFamily.isComplete()) {
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

QueueFamily LogicalDevice::findGraphicsFamily(VkPhysicalDevice device)
{
	QueueFamily graphicsFamily{};

	auto queueFamilies = getQueueFamilies(device);
	for (int i = 0; i < queueFamilies.size(); i++) {
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamily.index = i;
			return graphicsFamily;
		}
	}

	// graphics queue family wasn't found
	return graphicsFamily;
}

QueueFamily LogicalDevice::findPresentFamily(VkPhysicalDevice device, Surface& surface)
{
	QueueFamily presentFamily{};

	auto queueFamilies = getQueueFamilies(device);
	for (int i = 0; i < queueFamilies.size(); i++) {
		if (surface.supportsQueueFamily(device, i)) {
			presentFamily.index = i;
			return presentFamily;
		}
	}

	// present queue family wasn't found
	return presentFamily;
}

std::vector<VkDeviceQueueCreateInfo> LogicalDevice::getQueueCreateInfos(float* priorities)
{
	std::vector<VkDeviceQueueCreateInfo> createInfos{};

	VkDeviceQueueCreateInfo graphicsInfo{};
	graphicsInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphicsInfo.queueFamilyIndex = graphicsFamily.index.value();
	graphicsInfo.queueCount = 1;
	graphicsInfo.pQueuePriorities = priorities;
	createInfos.push_back(graphicsInfo);

	if (presentFamily.index.value() != graphicsFamily.index.value()) {
		VkDeviceQueueCreateInfo presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		presentInfo.queueFamilyIndex = presentFamily.index.value();
		presentInfo.queueCount = 1;
		presentInfo.pQueuePriorities = priorities;
		createInfos.push_back(presentInfo);
	}

	return createInfos;
}