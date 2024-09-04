/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/vk.c
 *
 * @brief A simple and easy-to-use Vulkan API in C
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#include <vulkan/vulkan.h>

VkApplicationInfo vk_linear_application_info(const char* pApplicationName) {
    VkApplicationInfo pApplicationInfo = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName   = pApplicationName,
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .pEngineName        = "linear",
        .engineVersion      = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .apiVersion         = VK_API_VERSION_1_0,
    };
    return pApplicationInfo;
}

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
