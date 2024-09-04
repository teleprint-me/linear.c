/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vk.h
 *
 * @brief A simple and easy-to-use Vulkan API in C
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef LINEAR_VK_H
#define LINEAR_VK_H

#include <vulkan/vulkan.h>

VkApplicationInfo vk_linear_application_info(const char* pApplicationName);

VkResult vk_linear_create_instance(
    const VkApplicationInfo* pApplicationInfo, VkInstance* pInstance
);

uint32_t
vk_linear_find_compute_queue_family_index(VkPhysicalDevice physicalDevice);

#endif // LINEAR_VK_H
