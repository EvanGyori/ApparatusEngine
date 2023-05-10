#include "VulkanInstance.h"

#include <cstring>

#include "DebugMessenger.h"

VulkanInstance::VulkanInstance() :
	handle(nullptr)
{
}

void VulkanInstance::init(const char* appName)
{
	// Specify the application info
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Apparatus Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Instance Create Info construction
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// TODO make debugging optional
	auto layers = getValidationLayers();
	if (!isValidationLayersSupported(layers)) {
		VK_CHECK(VK_ERROR_LAYER_NOT_PRESENT);
	}
	createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
	createInfo.ppEnabledLayerNames = layers.data();
	// Allows for debugging for the creation of the instance
	auto debugMessengerCreateInfo = DebugMessenger::getCreateInfo();
	createInfo.pNext = &debugMessengerCreateInfo;

	// Get required extensions and stop program if they aren't supported
	std::vector<const char*> extensions = getRequiredExtensions();
	if (!isExtensionsSupported(extensions)) {
		VK_CHECK(VK_ERROR_EXTENSION_NOT_PRESENT);
	}
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &handle); VK_CHECK(result);
}

VulkanInstance::~VulkanInstance()
{
	cleanup();
}

void VulkanInstance::cleanup()
{
	if (handle) {
		vkDestroyInstance(handle, nullptr);
		handle = nullptr;
	}
}

VkInstance VulkanInstance::getHandle()
{
	return handle;
}

std::vector<const char*> VulkanInstance::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// TODO make debugging optional
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

std::vector<VkExtensionProperties> VulkanInstance::getSupportedExtensions()
{
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	std::vector<VkExtensionProperties> extensions(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
	return extensions;
}

bool VulkanInstance::isExtensionsSupported(std::vector<const char*> extensions)
{
	std::vector<VkExtensionProperties> supportedExtensions = getSupportedExtensions();
	// Searches through each of the extensions and returns false if it isn't contained in supportedExtensions
	for (const char* extension : extensions) {
		bool found = false;
		for (VkExtensionProperties supportedExtension : supportedExtensions) {
			if (std::strcmp(extension, supportedExtension.extensionName) == 0) {
				found = true;
			}
		}

		if (!found) return false;
	}

	return true;
}

std::vector<const char*> VulkanInstance::getValidationLayers()
{
	return {"VK_LAYER_KHRONOS_validation"};
}

std::vector<VkLayerProperties> VulkanInstance::getSupportedValidationLayers()
{
	uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count, nullptr);
	std::vector<VkLayerProperties> layers(count);
	vkEnumerateInstanceLayerProperties(&count, layers.data());
	return layers;
}

bool VulkanInstance::isValidationLayersSupported(std::vector<const char*> layers)
{
	auto supportedLayers = getSupportedValidationLayers();
	for (const char* layer : layers) {
		bool found = false;
		for (VkLayerProperties supportedLayer : supportedLayers) {
			if (std::strcmp(layer, supportedLayer.layerName) == 0) {
				found = true;
			}
		}

		if (!found) return false;
	}

	return true;
}
