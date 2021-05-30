#pragma once

#include "common.h"
#include "memmap.h"

//! Timer struct, range: 0400:0100 - 0400:010F
/* The attribute is required, because union's counted as u32 otherwise.
*/
typedef struct
{
	union { uint16_t start, count; } PACKED;
	uint16_t cnt;
} ALIGN(4) tmr_rec_t;

extern const tmr_rec_t *tmr_rec;
