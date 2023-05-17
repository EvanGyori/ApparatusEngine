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
	/**
	 * @brief Default Constructor: Doesn't initilize the logical device, must call init
	 */
	LogicalDevice();

	/**
	 * @brief Creates a logical device which is a view of the specified physicalDevice.
	 * Enables the required extensions and creates queues for drawing and presenting.
	 * 
	 * @param _physicalDevice - the computer's physical device to use for graphics.
	 * use static member function findSuitablePhysicalDevice to locate a usable physical device
	 * @param surface - the surface that this logical device's queues will be presenting to
	 */
	void init(VkPhysicalDevice _physicalDevice, Surface& surface);

	/**
	 * @brief Destructor: Calls cleanup() to free up memory
	 */
	~LogicalDevice();

	/**
	 * @brief Destroys the logical device to free up memory.
	 * Do not use any vulkan objects created under this logical device once it has been destroyed.
	 */
	void cleanup();

	/**
	 * @brief Returns a physical device that supports all the neccessary details for use in graphics.
	 * 
	 * @param instance - used to locate all physical devices
	 * @param surface - the physical device must be compatible with the surface to be suitable
	 * 
	 * @return first physical device found that supports the required extensions, surface compatibility, and required queue families.
	 * nullptr if no physical device is found.
	 */
	static VkPhysicalDevice findSuitablePhysicalDevice(VulkanInstance& instance, Surface& surface);

	/**
	 * @brief Returns all physical devices on the computer
	 * 
	 * @param instance - the vulkan instance used to find physical devices
	 * 
	 * @return vector of physical devices
	 */
	static std::vector<VkPhysicalDevice> getPhysicalDevices(VulkanInstance& instance);

	/**
	 * @brief Returns whether the specified device supports the neccessary details for use in graphics
	 * 
	 * @param device - the physical device to check
	 * @param surface - the physical device must be compatible with the surface to be suitable
	 * 
	 * @return True if the physical device supports the required extensions, surface compatibility, and required queue families. False otherwise
	 */
	static bool isPhysicalDeviceSuitable(VkPhysicalDevice device, Surface& surface);

private:
	VkDevice handle;
	VkPhysicalDevice physicalDevice;
	QueueFamily graphicsFamily, presentFamily;

	/**
	 * @brief Returns the neccessary device extensions
	 * 
	 * @return vector of device extension names
	 */
	static std::vector<const char*> getRequiredExtensions();

	/**
	 * @brief Returns the extensions supported by the specified device
	 * 
	 * @param device - the physical device to find extensions under
	 * 
	 * @return vector of supported device extensions
	 */
	static std::vector<VkExtensionProperties> getSupportedExtensions(VkPhysicalDevice device);

	/**
	 * @brief Returns whether the specified device supports the specified extensions
	 * 
	 * @param device - the physical device used to get supported extensions
	 * @param extensions - the extensions to check for compatibility with the specified device
	 * 
	 * @return true if the extensions are supported by the device. False otherwise.
	 */
	static bool isExtensionsSupported(VkPhysicalDevice device, std::vector<const char*> extensions);

	static std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device);
	static QueueFamily findGraphicsFamily(VkPhysicalDevice device);
	static QueueFamily findPresentFamily(VkPhysicalDevice device, Surface& surface);

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