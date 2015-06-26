#include "mbed-alloc/ualloc.h"
#include "mbed/sbrk.h"
#include "cmsis.h"

extern void* dlmalloc(size_t);
extern void  dlfree(void*);
extern void* dlcalloc(size_t, size_t);
extern void* dlrealloc(void*, size_t);


void * ualloc(size_t bytes, UAllocTraits_t traits)
{
    void * ptr = NULL;
    if (traits.flags == UALLOC_TRAITS_NEVER_FREE) {
        ptr = krbs(bytes);
    } else if (traits.flags == UALLOC_TRAITS_ZERO_FILL) {
        ptr = dlcalloc(1, bytes);
    } else if (!(traits.flags & ~UALLOC_TRAITS_BITMASK)) {
        ptr = dlmalloc(bytes);
    }
    if(ptr == NULL) {
        __BKPT();
    }
    return ptr;
}
void * urealloc(void * ptr, size_t bytes, UAllocTraits_t traits)
{
    if(traits.flags & ~UALLOC_TRAITS_BITMASK) {
        // Traits not supported in urealloc yet
        __BKPT();
    }
    return dlrealloc(ptr, bytes);
}
void ufree(void * ptr)
{
    dlfree(ptr);
}
