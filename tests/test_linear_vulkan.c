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

#include <vulkan/vulkan.h>

int main(void) {
    return 0;
}
