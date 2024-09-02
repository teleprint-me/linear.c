/**
 * @file tests/test_linear_vulkan.c
 *
 * this is just a dry run for prototyping some experimental ideas. the goal is
 * to create 2 vectors and then perform vector addition using the vulkan api.
 *
 * the proof of concept should be simple, to the point, and utilize C and it's
 * builtin libraries. The only other inclusion allowed is that of the necessary
 * dependencies for including the vulkan headers themselves. otherwise, it must
 * be written from scratch unless this is unreasonable. however, this should be
 * feasible. A simple shader should be utilized to allow the computations to
 * occur. there is no need to create a window and render anything to that
 * window. this should be a purely headless implementation. as a result, this
 * should produce a minimal implementation.
 *
 * considerations:
 * - it must be a pure c implementation
 * - i should only need the basic vulkan header
 * - i'll need to figure out how to compile to plan out builds.
 * - the implementation assumes no graphical requirements
 *   - the vector/matrix/tensor api needs to be completely headless
 *   - should be able to execute as a standalone app within a cli term
 * - i'll require shaders for computations
 * - minimal dependencies (3rd party dependencies are not allowed)
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
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext                = NULL;
    createInfo.flags                = 0;
    createInfo.pApplicationInfo     = NULL; // Application info is optional

    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    assert(result == VK_SUCCESS);

    // 2. Select a physical device (GPU) that supports compute operations
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);
    assert(gpuCount > 0);

    VkPhysicalDevice* gpus = malloc(sizeof(VkPhysicalDevice) * gpuCount);
    vkEnumeratePhysicalDevices(instance, &gpuCount, gpus);
    VkPhysicalDevice physicalDevice
        = gpus[0]; // Simplest case: use the first GPU found

    // Additional setup such as checking compute capabilities, creating a
    // logical device, creating buffers, setting up descriptor sets, writing
    // shaders, etc.

    // Cleanup
    vkDestroyInstance(instance, NULL);
    free(gpus);

    return 0;
}
