#pragma once

#include <stdint.h>

//! Force a variable to an \a n-byte boundary
#define ALIGN(n)	__attribute__((aligned(n)))

//! Pack aggregate members
#define PACKED		__attribute__((packed))

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef __cplusplus
//! Boolean type
typedef enum { false, true } bool;
#endif

#ifndef BOOL
typedef uint8_t BOOL;
#define TRUE 1
#define FALSE 0
#endif

