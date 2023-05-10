#include "Surface.h"

#include "DebugMessenger.h"

Surface::Surface() :
	handle(nullptr)
{
}

void Surface::init(VulkanInstance& instance, Window& window)
{
	// TODO test for nullptr instanceHandle
	instanceHandle = instance.getHandle();
	VkResult result = glfwCreateWindowSurface(instanceHandle, window.getHandle(), nullptr, &handle); VK_CHECK(result);
}

Surface::~Surface()
{
	cleanup();
}

void Surface::cleanup()
{
	if (instanceHandle && handle) {
		vkDestroySurfaceKHR(instanceHandle, handle, nullptr);
		instanceHandle = nullptr;
		handle = nullptr;
	}
}