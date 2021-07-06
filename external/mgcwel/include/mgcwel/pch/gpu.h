#pragma once

#include "void.h"

#ifdef MW_ENABLE_GPU_SYNC
#   define MW_SYNC_GPU() glFinish()
#else
#   define MW_SYNC_GPU() MW_VOID_ASSEMBLY
#endif