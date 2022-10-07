
#ifndef _DPU_UTILS_H_
#define _DPU_UTILS_H_

#include <mram.h>


static uint32_t load(uint32_t ptr_m, uint32_t idx, uint32_t* cache_w) {
    // Load 8B
    uint32_t ptr_idx_m = ptr_m + idx*sizeof(uint32_t);
    uint32_t offset = ((uint32_t)ptr_idx_m)%8;
    uint32_t ptr_block_m = ptr_idx_m - offset;
    mram_read((__mram_ptr void const*)ptr_block_m, cache_w, 8);
    // Extract 4B
    uint32_t* cache_32_w = (uint32_t*) cache_w;
    return cache_32_w[offset/4];
}

static void store(uint32_t val, uint32_t ptr_m, uint32_t idx, uint32_t* cache_w) {
    // Load 8B
    uint32_t ptr_idx_m = ptr_m + idx*sizeof(uint32_t);
    uint32_t offset = ((uint32_t)ptr_idx_m)%8;
    uint32_t ptr_block_m = ptr_idx_m - offset;
    mram_read((__mram_ptr void const*)ptr_block_m, cache_w, 8);
    // Modify 4B
    uint32_t* cache_32_w = (uint32_t*) cache_w;
    cache_32_w[offset/4] = val;
    // Write back 8B
    mram_write(cache_w, (__mram_ptr void*)ptr_block_m, 8);
}

#endif

