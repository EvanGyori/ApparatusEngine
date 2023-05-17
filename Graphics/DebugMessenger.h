#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

#include "VulkanInstance.h"

#define VK_CHECK(result) logError(result, __FILE__, __func__, __LINE__)

// Throws a runtime error if the result isn't VK_SUCCESS
void logError(VkResult result, std::string file, std::string func, int line);

// A proxy function that finds and calls the vulkan create function
static VkResult createDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerCreateInfoEXT const* createInfo,
	VkAllocationCallbacks* allocator,
	VkDebugUtilsMessengerEXT* handle);

// A proxy function that finds and calls the vulkan destroy function
static void destroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT handle,
	VkAllocationCallbacks* allocator);

class DebugMessenger
{
public:
	/**
	 * @brief Default Constructor: Doesn't initialize the DebugMessenger, must call init.
	 */
	DebugMessenger();

	/**
	 * @brief Initializes the debug messenger to report messages from validation layers
	 * enabled in the specified instance.
	 * 
	 * The instance must be initialized or an error will be thrown.
	 *
	 * @param instance - the vulkan instance to create this object under
	*/
	void init(VulkanInstance& instance);

	/**
	 * @brief Destructor: Calls cleanup() to free up memory
	 */
	~DebugMessenger();

	/**
	 * @brief Destroys the debug messenger to free up memory.
	 */
	void cleanup();

	/**
	 * @brief Get the create info for the debug messenger, can be used to debug instance creation
	 * by passing it through pNext in VkInstanceCreateInfo.
	 */
	static VkDebugUtilsMessengerCreateInfoEXT getCreateInfo();

private:
	// The instance this object was created under
	VkInstance instanceHandle;
	VkDebugUtilsMessengerEXT handle;

	static std::string messageSeverityToString(VkDebugUtilsMessageSeverityFlagBitsEXT severity);
	static std::string messageTypeToString(VkDebugUtilsMessageTypeFlagsEXT type);

	// Called by the validation layers to report any information
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		VkDebugUtilsMessengerCallbackDataEXT const* callbackData,
		void* userData);
};