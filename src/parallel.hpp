#ifndef __PARALLEL_H__
#define __PARALLEL_H__

#include <functional>

/**
 * run for loop in parallel
 * 
 * @param length loop i in range [0, length - 1]
 * @param func func will be invoked like func(i)
 */
void parallel_for(int length, std::function<void(int)> func);

#endif
