#include "mbed-alloc/ualloc.h"
#include <stdlib.h>
#if defined(__ARMCC_VERSION)
#   define OVERRIDE_PREFIX(x)    $$Sub$$##x
#else
#   define OVERRIDE_PREFIX(x)    __wrap_##x
#endif


#if defined(__GNUC__)
void * __wrap__malloc_r(struct _reent *r, size_t size) {
    (void) r;
    UAllocTraits_t traits = {0};
    return ualloc(size, traits);
}
void * __wrap__calloc_r(size_t elements, size_t size) {
    UAllocTraits_t traits = {UALLOC_TRAITS_ZERO_FILL};
    return ualloc(elements*size, traits);
}
void * __wrap__realloc_r(struct _reent *r, void * ptr, size_t size) {
    (void)r;
    UAllocTraits_t traits = {0};
    return urealloc(ptr, size, traits);
}
void __wrap__free_r(struct _reent *r, void * ptr) {
    (void)r;
    ufree(ptr);
}
#elif defined(__ARMCC_VERSION)

#endif
