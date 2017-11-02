#ifndef _HEAP_REGION_DEBUG_H
#define _HEAP_REGION_DEBUG_H

#include "FreeRTOS.h"

#if (configENABLE_MEMORY_DEBUG == 1)

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_DOG_VALUE 0x1a2b3c4d
#define DEBUG_MAX_INFO_NUM 20
#define DEBUG_TYPE_MALLOC 1
#define DEBUG_TYPE_FREE   2

typedef struct {
    unsigned int dog;
    TaskHandle_t task;
    //unsigned int pc;
}block_head_t;

typedef struct {
    unsigned int dog;
}block_tail_t;

/* Please keep this definition same as BlockLink_t */
typedef struct _os_block_t {
    struct _os_block_t *next;               /*<< The next free block in the list. */
    int size: 24;                           /*<< The size of the free block. */
    int xtag: 7;                            /*<< Tag of this region */
    int xAllocated: 1;                      /*<< 1 if allocated */
}os_block_t;

typedef struct {
    block_head_t head;
    os_block_t os_block;
}debug_block_t;

typedef struct _mem_dbg_info{
    void     *addr;
    TaskHandle_t task;
    uint32_t pc;
    uint32_t time;
    uint8_t  type;
}mem_dbg_info_t;

typedef struct _mem_dbg_ctl{
    mem_dbg_info_t info[DEBUG_MAX_INFO_NUM];
    uint32_t cnt;
}mem_dbg_ctl_t;

#define BLOCK_HEAD_LEN sizeof(block_head_t)
#define BLOCK_TAIL_LEN sizeof(block_tail_t)
#define OS_BLOCK(_b)   ((os_block_t*)((debug_block_t*)((char*)(_b) + BLOCK_HEAD_LEN)))
#define DEBUG_BLOCK(_b)  ((debug_block_t*)((char*)(_b) - BLOCK_HEAD_LEN))
#define HEAD_DOG(_b) ((_b)->head.dog)
#define TAIL_DOG(_b) (*(unsigned int*)((char*)(_b) + (((_b)->os_block.size ) - BLOCK_TAIL_LEN)))

#define DOG_ASSERT()\
{\
    mem_debug_show();\
    abort();\
}

extern void mem_check_block(void * data);
extern void mem_init_dog(void *data);
extern void mem_debug_init(size_t size, void *start, void *end, portMUX_TYPE *mutex);
extern void mem_malloc_block(void *data);
extern void mem_free_block(void *data);
extern void mem_check_all(void* pv);
extern void mem_debug_malloc_show(void);
#if (configENABLE_MEMORY_DEBUG_ABORT == 1)
extern void mem_malloc_set_abort(TaskHandle_t task, int size, int count);
#endif

#if (configENABLE_MEMORY_DEBUG_DUMP >= 1)
typedef struct {
    TaskHandle_t task;
    void* address;
    int size: 24;                           /*<< The size of the free block. */
    int xtag: 7;                            /*<< Tag of this region */
    int xAllocated: 1;                      /*<< 1 if allocated */
} mem_dump_block_t;

#define NUM_USED_TAGS 3
typedef struct {
    TaskHandle_t task;
    int before[NUM_USED_TAGS];
    int after[NUM_USED_TAGS];
} mem_dump_totals_t;

extern size_t mem_debug_malloc_dump(TaskHandle_t task, mem_dump_block_t* buffer, size_t size);
extern size_t mem_debug_malloc_dump_totals(mem_dump_totals_t* totals, size_t* ntotal, size_t max,
					   TaskHandle_t* tasks, size_t ntasks,
					   mem_dump_block_t* buffer, size_t size);
#endif

#ifdef __cplusplus
}
#endif

#else

#define mem_check_block(...)
#define mem_init_dog(...)

#define BLOCK_HEAD_LEN 0
#define BLOCK_TAIL_LEN 0

#endif

#endif
