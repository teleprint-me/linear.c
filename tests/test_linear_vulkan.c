/**
 * @file tests/test_linear_vulkan.c
 *
 * @note keep fixtures and related tests as simple as reasonably possible.
 *       The simpler, the better.
 *
 * this is just a dry run for prototyping some experimental ideas. the goal is
 * to create 2 vectors and then perform vector addition using the vulkan api.
 *
 * the proof of concept should be simple, to the point, and utilize C and it's
 * builtin libraries. The only other inclusion allowed is that of the necessary
 * dependencies for including the vulkan headers themselves. otherwise, it must
 * be written from scratch.
 *
 * Manual Build:
 *     gcc -o test_linear_vulkan test_linear_vulkan.c -lvulkan
 * Manual Run:
 *     ./test_linear_vulkan
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

// Helper functions for error handling, buffer creation, etc.,
// should be defined here.

int main(void) {
    // 1. Initialize Vulkan: Create Vulkan instance
    VkInstance           instance;
    VkInstanceCreateInfo createInfo = {
        .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext            = NULL,
        .flags            = 0,
        .pApplicationInfo = NULL, // Application info is optional
    };

    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    assert(result == VK_SUCCESS);

    // 2. Select a physical device (GPU) that supports compute operations
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);
    assert(gpuCount > 0);

    VkPhysicalDevice* gpus = malloc(sizeof(VkPhysicalDevice) * gpuCount);
    vkEnumeratePhysicalDevices(instance, &gpuCount, gpus);
    // Simplest case: use the first GPU found
    VkPhysicalDevice physicalDevice = gpus[0];

    // Additional setup such as checking compute capabilities, creating a
    // logical device, creating buffers, setting up descriptor sets, writing
    // shaders, etc.

    // Cleanup
    vkDestroyInstance(instance, NULL);
    free(gpus);

    return 0;
}
