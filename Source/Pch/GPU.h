#pragma once

#include "Void.h"

#ifdef NATURAFORGE_ENABLE_GPU_SYNC
# define NATURAFORGE_SYNC_GPU() glFinish()
#else
# define NATURAFORGE_SYNC_GPU() NATURAFORGE_VOID_ASSEMBLY
#endif