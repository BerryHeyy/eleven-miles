#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <optional>

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enable_validation_layers = false;
#else
    const bool enable_validation_layers = true;
#endif

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

GLFWwindow* window;
VkInstance vk_instance;
VkPhysicalDevice physical_device = VK_NULL_HANDLE;
VkDevice device;

VkQueue graphics_queue;

bool check_validation_layer_support()
{
    // Get available validation layers.
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    // Check if required validation layers are in available layers.
    for (const char* layer_name : validation_layers)
    {
        bool layer_found = false;

        for (const VkLayerProperties layer_properties : available_layers)
        {
            if (strcmp(layer_name, layer_properties.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) return false;
    }

    return true;
}

void init_window()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphics_family;
};

QueueFamilyIndices find_queue_families(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    // Check if at least one queue family exists that supports VK_QUEUE_GRAPHICS_BIT
    int i = 0;
    for (VkQueueFamilyProperties queue_family : queue_families) //TODO: implement an early exit.
    {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics_family = i;
        }

        i++;
    }

    return indices;
}

bool is_device_suitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    QueueFamilyIndices indices = find_queue_families(device);

    return indices.graphics_family.has_value(); // Gpu needs to have graphics queue family.
}

void pick_physical_device()
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

    if (device_count == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());

    for (VkPhysicalDevice device : devices)
    {
        if (is_device_suitable(device))
        {
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to find a suitable GPU.");
    }
}

void create_logical_device()
{
    QueueFamilyIndices indices = find_queue_families(physical_device);

    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = indices.graphics_family.value();
    queue_create_info.queueCount = 1;
    float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    // Specify features
    VkPhysicalDeviceFeatures device_features {};

    // Create device
    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.queueCreateInfoCount = 1;

    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount = 0;

    if (enable_validation_layers)
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    }
    else
    {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device.");
    }

    vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphics_queue);
}

void create_vulkan_instance()
{
    if (enable_validation_layers && !check_validation_layer_support())
    {
        throw std::runtime_error("One or more requested validation layers do not exist.");
    }

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Eleven Miles";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Setup extensions from the built in glfw function
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // Setup validation layers
    if (enable_validation_layers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        createInfo.ppEnabledLayerNames = validation_layers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    // Create instance
    if (vkCreateInstance(&createInfo, nullptr, &vk_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

void init_vulkan()
{
    create_vulkan_instance();
    // setupDebugMessenger();
    pick_physical_device();
    create_logical_device();
}

void start_main_loop()
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }
}

void cleanup() 
{
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(vk_instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

int main()
{
    init_window();
    init_vulkan();

    start_main_loop();

    cleanup();
    return 0;
}