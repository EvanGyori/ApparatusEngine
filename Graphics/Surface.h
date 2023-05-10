#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanInstance.h"
#include "Window.h"

class Surface
{
public:
	Surface();
	void init(VulkanInstance& instance, Window& window);

	~Surface();
	void cleanup();

private:
	VkInstance instanceHandle;
	VkSurfaceKHR handle;
};