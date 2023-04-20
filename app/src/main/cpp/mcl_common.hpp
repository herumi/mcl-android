#pragma once

//#define MCL_C_API
#ifdef MCL_C_API
#include <mcl/bn_c384_256.h>
#else
#define MCL_DONT_USE_XBYAK
#define MCL_MAX_BIT_SIZE 384
#define MCL_MAX_FP_BIT_SIZE 384
#define MCL_MAX_FR_BIT_SIZE 256
#include <mcl/bn.hpp>
#endif
