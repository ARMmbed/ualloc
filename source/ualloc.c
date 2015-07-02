#include "mbed-alloc/ualloc.h"
#include "mbed/sbrk.h"
#include "cmsis.h"

extern void* dlmalloc(size_t);
extern void  dlfree(void*);
extern void* dlcalloc(size_t, size_t);
extern void* dlrealloc(void*, size_t);

extern int printf(const char *, ...);

#ifndef DEBUG
#define DEBUG 1
#endif


// Set debug level to 0 until non-allocating printf is available
const UAllocDebug_t ualloc_debug_level = UALLOC_DEBUG_NONE;//(DEBUG?UALLOC_DEBUG_MAX:UALLOC_DEBUG_NONE);

const char ua_chars[] = "NFEWIL";

#define ualloc_debug(ADBG_LEVEL, fmt, ...)\
    do {\
        if (ADBG_LEVEL <= ualloc_debug_level && ADBG_LEVEL < UALLOC_DEBUG_MAX) {\
            printf("UAL:%c " fmt, ua_chars[ADBG_LEVEL], __VA_ARGS__);\
        }\
    } while (0)


#if defined(__ARMCC_VERSION)
    #define caller_addr() __builtin_return_address(0)
#elif defined(__GNUC__)
    #define caller_addr() __builtin_extract_return_addr(__builtin_return_address(0))
#else
    #define caller_addr() (NULL)
#endif

void * ualloc(size_t bytes, UAllocTraits_t traits)
{
    void * ptr = NULL;
    void * caller = (void*) caller_addr();
    if (traits.flags == UALLOC_TRAITS_NEVER_FREE) {
        ptr = krbs(bytes);
    } else if (traits.flags == UALLOC_TRAITS_ZERO_FILL) {
        ptr = dlcalloc(1, bytes);
    } else if (!(traits.flags & ~UALLOC_TRAITS_BITMASK)) {
        ptr = dlmalloc(bytes);
    }

    if(ptr == NULL) {
        ualloc_debug(UALLOC_DEBUG_ERROR, "ua c:%p fail\n", caller);
        __BKPT(0);
    } else {
        ualloc_debug(UALLOC_DEBUG_LOG, "ua c:%p m:%p\n", caller, ptr);
    }
    return ptr;
}
void * urealloc(void * ptr, size_t bytes, UAllocTraits_t traits)
{
    void * caller = (void*) caller_addr();
    void *newptr;
    if(traits.flags & ~UALLOC_TRAITS_BITMASK) {
        ualloc_debug(UALLOC_DEBUG_ERROR, "ua c:%p fail\n", caller);
        // Traits not supported in urealloc yet
        __BKPT(0);
    }
    newptr = dlrealloc(ptr, bytes);
    if(newptr == NULL) {
        ualloc_debug(UALLOC_DEBUG_ERROR, "ur c:%p m0:%p fail\n", caller, ptr);
        __BKPT(0);
    } else {
        ualloc_debug(UALLOC_DEBUG_LOG, "ur c:%p m0:%p m1:%p\n", caller, ptr, newptr);
    }
    return newptr;
}
void ufree(void * ptr)
{
    void * caller = (void*) caller_addr();
    if(ptr == NULL) {
        __BKPT(0);
    } else {
        ualloc_debug(UALLOC_DEBUG_LOG, "uf c:%p m:%p\n", caller, ptr);
    }
    dlfree(ptr);
}
