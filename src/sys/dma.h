#pragma once

#include "common.h"
#include "memmap.h"

//!	DMA struct; range: 0400:00B0 - 0400:00DF
typedef struct {
    const void *src;
    void *dst;
    uint32_t cnt;
} dma_rec_t;

extern const dma_rec_t *dma_rec;
