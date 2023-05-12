#include "LogicalDevice.h"

#include <cstring>

#include "DebugMessenger.h"

LogicalDevice::LogicalDevice() :
	handle(nullptr),
	physicalDevice(nullptr)
{
}

void LogicalDevice::init(VkPhysicalDevice _physicalDevice)
{
	physicalDevice = _physicalDevice;
	if (physicalDevice == nullptr) {
		VK_CHECK(VK_ERROR_INCOMPATIBLE_DRIVER);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
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

VkPhysicalDevice LogicalDevice::findSuitablePhysicalDevice(VulkanInstance& instance)
{
	auto physicalDevices = getPhysicalDevices(instance);
	for (auto device : physicalDevices) {
		if (isPhysicalDeviceSuitable(device)) {
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
	return physicalDevices
}

bool LogicalDevice::isPhysicalDeviceSuitable(VkPhysicalDevice device, Surface& surface)
{
	auto extensions = getRequiredExtensions();
	if (!isExtensionsSupported(device, extensions)) {
		return false;
	}

	// Check if the surface and physicalDevice supports the swapchain details needed
	if (surface.getFormats(device).isEmpty() || surface.getPresentModes(device).isEmpty()) {
		return false;
	}

	// Check if the device supports the needed queues and that the present queue supports presenting to the specific surface


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