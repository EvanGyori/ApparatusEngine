#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class VulkanInstance
{
public:
	/**
	 * @brief Default Constructor: Doesn't initialize the instance, must call init().
	 */
	VulkanInstance();

	/**
	 * @brief Creates the vulkan instance with the required GLFW extensions and standard validation layers.
	 * 
	 * Throws an error if the extensions or validation layers aren't found or creation fails.
	 * 
	 * @param appName - specifies the application's name to use for initializing the instance
	 */
	void init(const char* appName);

	/**
	 * @brief Destructor: Calls cleanup() to free up memory
	 */
	~VulkanInstance();

	/**
	 * @brief Destroys the vulkan instance to free up memory.
	 * Do not use any vulkan objects created under this instance once it has been destroyed.
	 */
	void cleanup();

	/**
	 * @brief Returns this instance's handle.
	 * 
	 * Only use for object creation and clean up, use this object for all other instance tasks.
	 * 
	 * @return the instance handle
	 */
	VkInstance getHandle();

private:
	VkInstance handle;

	/**
	 * @brief Returns the GLFW instance extensions and debug utils if needed
	 * 
	 * The given extensions may or may not be supported, be sure to check.
	 * 
	 * @return vector of instance extension names
	 */
	std::vector<const char*> getRequiredExtensions();
	
	/**
	 * @brief Returns all the supported instance extensions by the computer's hardware
	 */
	std::vector<VkExtensionProperties> getSupportedExtensions();

	/**
	 * @brief Returns whether the provided extensions are supported by the computer's hardware
	 */
	bool isExtensionsSupported(std::vector<const char*> extensions);

	/**
	 * @brief Returns the standard validation layer
	 */
	std::vector<const char*> getValidationLayers();

	/**
	 * @brief Returns supported validation layers
	 */
	std::vector<VkLayerProperties> getSupportedValidationLayers();

	/**
	 * @brief Checks if the specified layers are supported
	 */
	bool isValidationLayersSupported(std::vector<const char*> layers);
};