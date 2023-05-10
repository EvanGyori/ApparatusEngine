#include "DebugMessenger.h"

#include <iostream>

void logError(VkResult result, std::string file, std::string func, int line)
{
	if (result != VK_SUCCESS) {
		throw std::runtime_error("VULKAN ERROR: " + std::to_string(result) + ", file: " + file + ", func: " + func + ", line: " + std::to_string(line));
	}
}

VkResult createDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerCreateInfoEXT const* createInfo,
	VkAllocationCallbacks* allocator,
	VkDebugUtilsMessengerEXT* handle)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, createInfo, allocator, handle);
	}
	// Returns an error if the function wasn't found
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void destroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT handle,
	VkAllocationCallbacks* allocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, handle, allocator);
	} else {
		// Shouldn't stop program
		// TODO
		// VK_CHECK(VK_ERROR_EXTENSION_NOT_PRESENT);
	}
}

DebugMessenger::DebugMessenger() :
	instanceHandle(nullptr),
	handle(nullptr)
{
}

void DebugMessenger::init(VulkanInstance& instance)
{
	instanceHandle = instance.getHandle();
	auto createInfo = getCreateInfo();
	// TODO test for nullptr instanceHandle
	VkResult result = createDebugUtilsMessengerEXT(instanceHandle, &createInfo, nullptr, &handle); VK_CHECK(result);
}

DebugMessenger::~DebugMessenger()
{
	cleanup();
}

void DebugMessenger::cleanup()
{
	if (handle && instanceHandle) {
		destroyDebugUtilsMessengerEXT(instanceHandle, handle, nullptr);
		handle = nullptr;
		instanceHandle = nullptr;
	}
}

VkDebugUtilsMessengerCreateInfoEXT DebugMessenger::getCreateInfo()
{
	// Specify which messages are received by the validation layers
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		//VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;

	return createInfo;
}

std::string DebugMessenger::messageSeverityToString(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
{
	switch (severity) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		return "verbose";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		return "info";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		return "WARNING";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		return "ERROR";
		break;
	}

	return "UNKNOWN SEVERITY";
}

std::string DebugMessenger::messageTypeToString(VkDebugUtilsMessageTypeFlagsEXT type)
{
	switch (type) {
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		return "general";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		return "validation";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		return "performance";
		break;
	}

	return "UNKNOWN TYPE";
}

VkBool32 DebugMessenger::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	VkDebugUtilsMessengerCallbackDataEXT const* callbackData,
	void* userData)
{
	std::cout << messageSeverityToString(messageSeverity) << ", " << messageTypeToString(messageType) << ", "
		<< callbackData->pMessage << '\n';

	return VK_FALSE;
}