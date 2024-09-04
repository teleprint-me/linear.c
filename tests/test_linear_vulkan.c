/**
 * Copyright © 2024 Austin Berrio
 *
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
#include <string.h>

#include <vulkan/vulkan.h>

VkApplicationInfo vk_linear_application_info(const char* pApplicationName) {
    VkApplicationInfo appInfo = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName   = pApplicationName,
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .pEngineName        = pApplicationName,
        .engineVersion      = VK_MAKE_API_VERSION(0, 1, 0, 0),
        .apiVersion         = VK_API_VERSION_1_0,
    };
    return appInfo;
}

VkResult vk_linear_create_instance(
    const VkApplicationInfo* pApplicationInfo, VkInstance* pInstance
) {
    VkInstanceCreateInfo createInfo = {
        .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext            = NULL,
        .flags            = 0,
        .pApplicationInfo = pApplicationInfo,
    };

    VkResult result = vkCreateInstance(&createInfo, NULL, pInstance);
    assert(result == VK_SUCCESS);
    return result;
}

// Helper functions to find a compute queue family index
uint32_t findComputeQueueFamilyIndex(VkPhysicalDevice physicalDevice) {
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice, &queueFamilyCount, NULL
    );
    VkQueueFamilyProperties* queueFamilies
        = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice, &queueFamilyCount, queueFamilies
    );

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            free(queueFamilies);
            return i;
        }
    }

    free(queueFamilies);
    assert(0 && "No compute queue family found.");
    return -1;
}

int main(void) {
    // 1. Initialize Vulkan: Create Vulkan instance
    VkInstance instance;

    // Create application info directly on the stack
    VkApplicationInfo appInfo = vk_linear_application_info("linear");

    // Create the Vulkan instance
    VkResult result = vk_linear_create_instance(&appInfo, &instance);

    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance!\n");
        return EXIT_FAILURE;
    }

    // 2. Select a physical device (GPU) that supports compute operations
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);
    assert(gpuCount > 0);

    VkPhysicalDevice* gpus = malloc(sizeof(VkPhysicalDevice) * gpuCount);
    vkEnumeratePhysicalDevices(instance, &gpuCount, gpus);

    // Simplest case: use the first GPU found
    VkPhysicalDevice physicalDevice = gpus[0];

    // 3. Create a logical device with a compute queue
    uint32_t computeQueueFamilyIndex
        = findComputeQueueFamilyIndex(physicalDevice);

    float                   queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo
        = {.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
           .queueFamilyIndex = computeQueueFamilyIndex,
           .queueCount       = 1,
           .pQueuePriorities = &queuePriority};

    VkDeviceCreateInfo deviceCreateInfo
        = {.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
           .queueCreateInfoCount = 1,
           .pQueueCreateInfos    = &queueCreateInfo};

    VkDevice device;
    result = vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device);
    assert(result == VK_SUCCESS);

    // 4. Get a handle to the compute queue
    VkQueue computeQueue;
    vkGetDeviceQueue(device, computeQueueFamilyIndex, 0, &computeQueue);

    // 5. Create buffers for input/output vectors
    // variable-sized object may not be initialized except with an empty
    // initializer. it's better to use a definition, static initialization via
    // the stack, or explicit dynamic allocation via the heap.
    // @note we can use the vector api we implemented in linear to do this for
    // us. vector_t*a = vector_create(4); then access the contents as needed
    // a->data = {1.0f, 2.0f, 3.0f, 4.0f}; // data is a pointer to a float
    // a->columns; // length of the data array
    float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};
    float resultVector[4];

    // Here you would allocate and bind Vulkan buffers, such as for
    // InputBufferA, InputBufferB, and OutputBuffer. For brevity, this is left
    // as comments since it's detailed and specific to the Vulkan memory
    // allocation.

    // 6. Create a compute pipeline
    // This includes loading the SPIR-V shader, creating shader modules, and
    // setting up the pipeline. Assuming you have compiled
    // 'test_linear_shader.comp' to SPIR-V, you would load it here.

    // 7. Create descriptor sets and bind buffers

    // 8. Record command buffer

    // 9. Submit commands to the compute queue

    // 10. Retrieve results from GPU

    // Cleanup Vulkan resources
    vkDestroyDevice(device, NULL);
    vkDestroyInstance(instance, NULL);
    free(gpus);

    return 0;
}
