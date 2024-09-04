/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/vk.c
 *
 * @brief A simple and easy-to-use Vulkan API in C
 *
 * @ref https://docs.vulkan.org/
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#include "vk.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

// Helper function to create a application info object
VkApplicationInfo vk_linear_application_info(const char* pApplicationName) {
    VkApplicationInfo pApplicationInfo = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName   = pApplicationName,
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 3, 0),
        .pEngineName        = "linear",
        .engineVersion      = VK_MAKE_API_VERSION(0, 1, 3, 0),
        .apiVersion         = VK_API_VERSION_1_3,
    };
    return pApplicationInfo;
}

// Helper function to create a vulkan instance object
VkResult vk_linear_create_instance(
    const VkApplicationInfo* pApplicationInfo, VkInstance* pInstance
) {
    VkInstanceCreateInfo pCreateInfo = {
        .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext            = NULL,
        .flags            = 0,
        .pApplicationInfo = pApplicationInfo,
    };

    VkResult result = vkCreateInstance(&pCreateInfo, NULL, pInstance);
    assert(result == VK_SUCCESS);
    return result;
}

// Helper function to find a compute queue family index
uint32_t
vk_linear_find_compute_queue_family_index(VkPhysicalDevice physicalDevice) {
    uint32_t pQueueFamilyPropertyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice, &pQueueFamilyPropertyCount, NULL
    );

    VkQueueFamilyProperties* pQueueFamilyProperties
        = malloc(sizeof(VkQueueFamilyProperties) * pQueueFamilyPropertyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice, &pQueueFamilyPropertyCount, pQueueFamilyProperties
    );

    for (uint32_t i = 0; i < pQueueFamilyPropertyCount; i++) {
        if (pQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            free(pQueueFamilyProperties);
            return i;
        }
    }

    free(pQueueFamilyProperties);
    assert(0 && "No compute queue family found.");
    return -1;
}
