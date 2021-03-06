#pragma once
#include <wut.h>

/**
 * \defgroup coreinit_expheap Expanded Heap
 * \ingroup coreinit
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMExpandedHeap MEMExpandedHeap;;

struct MEMExpandedHeap
{
};
UNKNOWN_SIZE(MEMExpandedHeap);

typedef enum MEMExpHeapMode
{
   MEM_EXP_HEAP_MODE_FIRST_FREE     = 0,
   MEM_EXP_HEAP_MODE_NEAREST_SIZE   = 1,
} MEMExpHeapMode;

typedef enum MEMExpHeapDirection
{
   MEM_EXP_HEAP_DIR_FROM_TOP        = 0,
   MEM_EXP_HEAP_DIR_FROM_BOTTOM     = 1,
} MEMExpHeapDirection;

MEMExpandedHeap *
MEMCreateExpHeapEx(MEMExpandedHeap *heap, uint32_t size, uint16_t flags);

MEMExpandedHeap *
MEMDestroyExpHeap(MEMExpandedHeap *heap);

void *
MEMAllocFromExpHeapEx(MEMExpandedHeap *heap, uint32_t size, int alignment);

void
MEMFreeToExpHeap(MEMExpandedHeap *heap, uint8_t *block);

MEMExpHeapMode
MEMSetAllocModeForExpHeap(MEMExpandedHeap *heap, MEMExpHeapMode mode);

MEMExpHeapMode
MEMGetAllocModeForExpHeap(MEMExpandedHeap *heap);

uint32_t
MEMAdjustExpHeap(MEMExpandedHeap *heap);

uint32_t
MEMResizeForMBlockExpHeap(MEMExpandedHeap *heap, uint8_t *address, uint32_t size);

uint32_t
MEMGetTotalFreeSizeForExpHeap(MEMExpandedHeap *heap);

uint32_t
MEMGetAllocatableSizeForExpHeapEx(MEMExpandedHeap *heap, int alignment);

uint16_t
MEMSetGroupIDForExpHeap(MEMExpandedHeap *heap, uint16_t id);

uint16_t
MEMGetGroupIDForExpHeap(MEMExpandedHeap *heap);

uint32_t
MEMGetSizeForMBlockExpHeap(uint8_t *addr);

uint16_t
MEMGetGroupIDForMBlockExpHeap(uint8_t *addr);

MEMExpHeapDirection
MEMGetAllocDirForMBlockExpHeap(uint8_t *addr);

#ifdef __cplusplus
}
#endif

/** @} */
