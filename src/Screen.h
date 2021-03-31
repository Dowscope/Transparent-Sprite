#ifndef SCREEN_H
#define SCREEN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <optional>
#include <set>

void static error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

struct _QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class Screen
{
private:
    const std::vector<const char*> _deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    GLFWwindow* _window;
    VkInstance _instance;
    VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    VkDevice _device;
    VkQueue _graphicsQueue;
    VkSurfaceKHR _surface;
    VkQueue _presentQueue;

    bool _initWindow();
    bool _initVulkan();
    VkResult _createInstance();
    bool _pickPhysicalDevice();
    bool _isDeviceSuitable(VkPhysicalDevice device);
    _QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);
    bool _createLogicalDevice();
    bool _createSurface();
    bool _checkDeviceExtensionSupport(VkPhysicalDevice device);
public:
    const int WIDTH = 800;
    const int HEIGHT = 600;
    bool hasInitialized = false;

    Screen();
    ~Screen();
    bool shouldWindowClose();
    void pollEvents();
};




#endif