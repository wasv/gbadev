#pragma once

#define BIT(n) (1 << (n))
#define BIT_SHIFT(a, n) ((a) << (n))
#define BIT_SET(word, flag) (word |= (flag))
#define BIT_CLEAR(word, flag) (word &= ~(flag))
#define BIT_FLIP(word, flag) (word ^= (flag))
#define BIT_EQ(word, flag) (((word) & (flag)) == (flag))

#define BFN_PREP(x, name) (((x) << name##_SHIFT) & name##_MASK)
#define BFN_GET(y, name) (((y)&name##_MASK) >> name##_SHIFT)
#define BFN_SET(y, x, name) (y = ((y) & ~name##_MASK) | BFN_PREP(x, name))

#define BFN_PREP2(x, name) ((x)&name##_MASK)
#define BFN_GET2(y, name) ((y)&name##_MASK)
#define BFN_SET2(y, x, name) (y = ((y) & ~name##_MASK) | BFN_PREP2(x, name))
