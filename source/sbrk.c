#include <stdint.h>
#include <stddef.h>
#include "cmsis.h"

#ifndef SBRK_ALIGN
#define SBRK_ALIGN 4U
#endif
#if (SBRK_ALIGN & (SBRK_ALIGN-1))
#error SBRK_ALIGN must be a power of 2
#endif


#ifndef SBRK_INC_MIN
#define SBRK_INC_MIN (SBRK_ALIGN)
#endif

#ifndef KRBS_ALIGN
#define KRBS_ALIGN 4U
#endif
#if (KRBS_ALIGN & (KRBS_ALIGN-1))
#error KRBS_ALIGN must be a power of 2
#endif

#ifndef KRBS_INC_MIN
#define KRBS_INC_MIN (KRBS_ALIGN)
#endif

extern uint32_t KRBS_START;
extern uint32_t SBRK_START;

volatile void * krbs_ptr = &KRBS_START;
volatile void * sbrk_ptr = &SBRK_START;
volatile uintptr_t sbrk_diff = 4096U;

void * sbrk(const size_t size) {
    uintptr_t sbrk_tmp = NULL;
    size_t size_internal = size;
    // Guarantee minimum allocation size
    if (size_internal < SBRK_INC_MIN) {
        size_internal = SBRK_INC_MIN;
    }
    size_internal = ( size_internal + SBRK_ALIGN - 1) & ~(SBRK_ALIGN - 1);

    while(1) {
        uintptr_t ptr_diff = __LDREXW((uint32_t *)&sbrk_diff);
        if (size_internal > ptr_diff) {
            return NULL;
        }
        ptr_diff -= size_internal;
        if (__STREXW(ptr_diff, (uint32_t *)&sbrk_diff))
            continue;
        break;
    }

    while (1) {
        sbrk_tmp = __LDREXW((uint32_t *)&sbrk_ptr);
        // store the base pointer for the allocated memory
        const uintptr_t sbrk_old = sbrk_tmp;
        // Calculate the new krbs pointer
        sbrk_tmp += size_internal;
        // Guarantee krbs alignment alignment
        if(__STREXW(sbrk_tmp, (uint32_t *)&sbrk_ptr))
            continue;
        // restore the base pointer of the allocated memory
        sbrk_tmp = sbrk_old;
        break;
    }
    return (void *) sbrk_tmp;
}

void * krbs(const size_t size) {
    uintptr_t krbs_tmp = NULL;
    size_t size_internal = size;
    // Guarantee minimum allocation size
    if (size_internal < KRBS_INC_MIN) {
        size_internal = KRBS_INC_MIN;
    }
    size_internal = (size_internal + KRBS_ALIGN - 1) & ~(KRBS_ALIGN - 1);

    while(1) {
        uintptr_t ptr_diff = __LDREXW((uint32_t *)&sbrk_diff);
        if (size_internal > ptr_diff) {
            return NULL;
        }
        ptr_diff -= size_internal;
        if (__STREXW(ptr_diff, (uint32_t *)&sbrk_diff))
            continue;
        break;
    }

    while (1) {
        krbs_tmp = __LDREXW((uint32_t *)&krbs_ptr);
        // Calculate the new krbs pointer
        krbs_tmp -= size_internal;
        // Guarantee krbs alignment alignment
        if(__STREXW(krbs_tmp, (uint32_t *)&krbs_ptr))
            continue;
        break;
    }
    return (void *) krbs_tmp;
}
