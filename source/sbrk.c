#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "cmsis.h"
#include "mbed-alloc/sbrk.h"

volatile void * krbs_ptr = &KRBS_START;
volatile void * sbrk_ptr = &SBRK_START;
volatile ptrdiff_t sbrk_diff = HEAP_SIZE;

caddr_t mbed_sbrk(const ptrdiff_t size);

caddr_t __wrap__sbrk(const ptrdiff_t size) {
    return mbed_sbrk(size);
}

caddr_t mbed_sbrk(const ptrdiff_t size)
{
    uintptr_t sbrk_tmp = NULL;
    size_t size_internal = size;
    if (size == 0) {
        return (void *) sbrk_ptr;
    }
    // Guarantee minimum allocation size
    if (size_internal < SBRK_INC_MIN) {
        size_internal = SBRK_INC_MIN;
    }
    size_internal = ( size_internal + SBRK_ALIGN - 1) & ~(SBRK_ALIGN - 1);

    while(1) {
        uintptr_t ptr_diff = __LDREXW((uint32_t *)&sbrk_diff);
        if (size_internal > ptr_diff) {
            return (void*)-1;
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

void * krbs(const ptrdiff_t size)
{
    return krbs_ex(size, NULL);
}

void * krbs_ex(const ptrdiff_t size, ptrdiff_t *actual)
{
    uintptr_t krbs_tmp = NULL;
    size_t size_internal = size;
    if (size == 0) {
        return (void *) krbs_ptr;
    }
    // Guarantee minimum allocation size
    if (size_internal < KRBS_INC_MIN) {
        size_internal = KRBS_INC_MIN;
    }
    size_internal = (size_internal + KRBS_ALIGN - 1) & ~(KRBS_ALIGN - 1);

    while(1) {
        uintptr_t ptr_diff = __LDREXW((uint32_t *)&sbrk_diff);
        if (size_internal > ptr_diff && actual == NULL) {
            return (void*)-1;
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
