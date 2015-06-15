#include <stdint.h>
#include <stddef.h>

#ifndef SBRK_ALIGN
#define SBRK_ALIGN 4U
#endif

#ifndef SBRK_INC_MIN
#define SBRK_INC_MIN (SBRK_ALIGN)
#endif

#ifndef KRBS_ALIGN
#define KRBS_ALIGN 4U
#endif

#ifndef KRBS_INC_MIN
#define KRBS_INC_MIN (KRBS_ALIGN)
#endif

extern const void * KRBS_START;
extern const void * SBRK_START;

volatile void * krbs_ptr = KRBS_START;
volatile void * sbrk_ptr = SBRK_START;

void * sbrk(const size_t size) {
    uintptr_t sbrk_tmp;
    size_t size_internal = size
    // Guarantee minimum allocation size
    if (size_internal < SBRK_INC_MIN) {
        size_internal = SBRK_INC_MIN
    }

    do {
        sbrk_tmp = __LDREXW(&sbrk_ptr);
        const uintptr_t krbs_tmp = __LDREXW(&krbs_ptr);
        // store the base pointer for the allocated memory
        const uintptr_t sbrk_old = sbrk_tmp;
        // Calculate the new krbs pointer
        sbrk_tmp += size_internal;
        // Guarantee krbs alignment alignment
        sbrk_tmp = ( sbrk_tmp + SBRK_ALIGN - 1) & ~(SBRK_ALIGN - 1)

        if (krbs_tmp < sbrk_tmp) {
            sbrk_tmp = NULL;
            break;
        }
        // krbs_tmp has not changed, so if write succeeds,
        // but sbrk_tmp fails, there are no side-effects
        if(__STREXW(krbs_tmp, &krbs_ptr))
            continue;
        if(__STREXW(sbrk_tmp, &sbrk_ptr))
            continue;
        // restore the base pointer of the allocated memory
        sbrk_tmp = sbrk_old;
    } while( 0 );

    return (void *) sbrk_tmp;
}

void * krbs(const size_t size) {
    size_t size_internal = size
    uintptr_t krbs_tmp;
    // Guarantee minimum allocation size
    if (size_internal < KRBS_INC_MIN) {
        size_internal = KRBS_INC_MIN
    }

    do {
        krbs_tmp = __LDREXW(&krbs_ptr);
        const uintptr_t sbrk_tmp = __LDREXW(&sbrk_ptr);

        // Calculate the new krbs pointer
        krbs_tmp -= size_internal;
        // Guarantee krbs alignment alignment
        krbs_tmp = krbs_tmp & ~( ((uintptr_t) 1 << KRBS_ALIGN) - 1 );

        if (krbs_tmp < sbrk_tmp) {
            krbs_tmp = NULL;
            break;
        }
        // sbrk_tmp has not changed, so if write succeeds,
        // but krbs_tmp fails, there are no side-effects
        if(__STREXW(sbrk_tmp, &sbrk_ptr))
            continue;
        if(__STREXW(krbs_tmp, &krbs_ptr))
            continue;
    } while( 0 );

    return (void *) krbs_tmp;
}
