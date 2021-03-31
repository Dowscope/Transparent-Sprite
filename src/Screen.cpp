#include "Screen.h"

Screen::Screen()
{
    hasInitialized = _initWindow() && _initVulkan();
}

Screen::~Screen()
{
    if(_device) vkDestroyDevice(_device, nullptr);
    if(_surface) vkDestroySurfaceKHR(_instance, _surface, nullptr);
    if(_instance) vkDestroyInstance(_instance, nullptr);
    if(_window) glfwDestroyWindow(_window);
    glfwTerminate();
}

bool Screen::_initWindow()
{
    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
    {
        std::cout << "SCREEN.CPP: GLFW did not initialize" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = glfwCreateWindow(WIDTH, HEIGHT, "OPENGL", nullptr, nullptr);
    if(!_window)
    {
        std::cout << "SCREEN.CPP: GLFW window did not create" << std::endl;
        return false;
    }

    return true;
}

bool Screen::_initVulkan(){
    VkResult instanceResult = _createInstance();
    if (instanceResult != VK_SUCCESS){
        std::cout << "SCREEN.CPP: Vulkan instance did not create - Error: " << instanceResult << std::endl;
        return false;
    }

    if (!_createSurface()){
        std::cout << "SCREEN.CPP: Surface creation error" << std::endl;
        return false;
    }

    if (!_pickPhysicalDevice()){
        std::cout << "SCREEN.CPP: Vulkan physical device error" << std::endl;
        return false;
    }

    if (!_createLogicalDevice()){
        std::cout << "SCREEN.CPP: Vulkan logical device error" << std::endl;
        return false;
    }

    

    return true;
}

VkResult Screen::_createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    return vkCreateInstance(&createInfo, nullptr, &_instance);
}

bool Screen::_pickPhysicalDevice(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

    if(deviceCount == 0) {
        std::cout << "SCREEN.CPP: Vulkan physical device not found" << std::endl;
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (_isDeviceSuitable(device)) {
            _physicalDevice = device;
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE) {
        std::cout << "SCREEN.CPP: Vulkan physical device not suitable" << std::endl;
        return false;
    }

    return true;
}

bool Screen::_isDeviceSuitable(VkPhysicalDevice device){
    _QueueFamilyIndices indices = _findQueueFamilies(device);
    bool extensionSupported = _checkDeviceExtensionSupport(device);
    return indices.isComplete() && extensionSupported;
}

_QueueFamilyIndices Screen::_findQueueFamilies(VkPhysicalDevice device) {
    _QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        i++;
    }

    return indices;
}

bool Screen::_createLogicalDevice(){
    _QueueFamilyIndices indices = _findQueueFamilies(_physicalDevice);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
        std::cout << "SCREEN.CPP: Failed to create logical device!" << std::endl;
        return false;
    }

    vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
    
    return true;
}

bool Screen::_createSurface(){
    if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS) {
        std::cout << "SCREEN.CPP: failed to create window surface!" << std::endl;
        return false;
    }
    return true;
}

bool Screen::_checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool Screen::shouldWindowClose()
{
    return glfwWindowShouldClose(_window);
}

void Screen::pollEvents()
{

}


