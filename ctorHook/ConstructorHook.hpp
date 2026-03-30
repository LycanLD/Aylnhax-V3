/**
 * @brief Hooks a constructor-like function to capture all 'this' instances into a vector.
 * 
 * This utility is useful in Android native hooking when you want to log or store all instances
 * created by a class constructor.
 * 
 * @param targetFunc Address of the target constructor function
 * @param container Reference to std::vector to store the captured instances (this pointer)
 */
 
#include <vector>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void hookConstructor(void *targetFunc, std::vector<void *> &container);

#ifdef __cplusplus
}
#endif


