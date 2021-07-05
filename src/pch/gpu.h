#pragma once

#include "void.h"

#ifdef NF_ENABLE_GPU_SYNC
# define NF_SYNC_GPU() glFinish()
#else
# define NF_SYNC_GPU() NF_VOID_ASSEMBLY
#endif