#include "Swapchain.h"

#include "DebugMessenger.h"

#include <limits>
#include <algorithm>

Swapchain::Swapchain() :
	handle(nullptr),
	deviceHandle(nullptr)
{
}

void Swapchain::init(LogicalDevice& device, Surface& surface, Window& window)
{
	deviceHandle = device.getHandle();
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface.getHandle();

	// Retrieve surface information
	auto surfaceCapabilities = surface.getCapabilities(device.getPhysicalDevice));
	auto surfaceFormats = surface.getFormats(device.getPhysicalDevice));

	// Pick min image count
	uint32_t minImageCount = pickMinImageCount(capabilities);
	createInfo.minImageCount = minImageCount;

	// Pick surface format
	VkSurfaceFormatKHR surfaceFormat = pickFormat(surfaceFormats);
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;

	// Pick image extent
	createInfo.imageExtent = pickExtent(capabilities, window);

	// image specifics
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VkResult result = vkCreateSwapchainKHR(deviceHandle, &createInfo, nullptr, &handle); VK_CHECK(result);

	// TODO retrieve images
}

Swapchain::~Swapchain()
{
	cleanup();
}

void Swapchain::cleanup()
{
	if (handle && deviceHandle) {
		vkDestroySwapchainKHR(deviceHandle, handle, nullptr);
		handle = nullptr;
		deviceHandle = nullptr;
	}
}

uint32_t Swapchain::pickMinImageCount(VkSurfaceCapabilitiesKHR capabilities)
{
	uint32_t count = 2;
	if (count < capabilities.minImageCount) {
		count = capabilities.minImageCount;
	} else if (capabilities.maxImageCount != 0 && count > capabilities.maxImageCount) {
		count = capabilities.maxImageCount;
	}

	return count;
}

VkExtent2D Swapchain::pickExtent(VkSurfaceCapabilitiesKHR capabilities, Window& window)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		int width, height;
		glfwGetFramebufferSize(window.getHandle(), &width, &height);

		VkExtent2D extent {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return extent;
	}
}