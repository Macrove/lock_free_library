#pragma once
#include<stddef.h>
#include<assert.h>

#define CACHE_LINE_SIZE 64

inline size_t ceil_pow_of_2(size_t val){
    assert(val > 2);
    val --;
    val |= (val >> 1);
    val |= (val >> 2);
    val |= (val >> 4);
    val |= (val >> 8);
    val |= (val >> 16);
#if SIZE_MAX > UINT32_MAX
    val |= (val >> 32);
#endif
    return val+1;
}
