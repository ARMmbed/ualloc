#include "mbed-alloc/ualloc.h"
#include <stdlib.h>

#if defined(__ARMCC_VERSION)
void * malloc_c_force_import;
void * $Sub$$malloc(size_t size) {
    UAllocTraits_t traits = {0};
    return mbed_ualloc(size, traits);
}
void * $Sub$$calloc(size_t elements, size_t size) {
    UAllocTraits_t traits = {UALLOC_TRAITS_ZERO_FILL};
    return mbed_ualloc(elements*size, traits);
}
void * $Sub$$realloc(void * ptr, size_t size) {
    UAllocTraits_t traits = {0};
    return mbed_urealloc(ptr, size, traits);
}
void $Sub$$free(void * ptr) {
    mbed_ufree(ptr);
}
#elif defined(__GNUC__)
void * __wrap__malloc_r(struct _reent *r, size_t size) {
    (void) r;
    UAllocTraits_t traits = {0};
    return mbed_ualloc(size, traits);
}
void * __wrap__calloc_r(struct _reent *r, size_t elements, size_t size) {
    (void) r;
    UAllocTraits_t traits = {UALLOC_TRAITS_ZERO_FILL};
    return mbed_ualloc(elements*size, traits);
}
void * __wrap__realloc_r(struct _reent *r, void * ptr, size_t size) {
    (void)r;
    UAllocTraits_t traits = {0};
    return mbed_urealloc(ptr, size, traits);
}
void __wrap__free_r(struct _reent *r, void * ptr) {
    (void)r;
    mbed_ufree(ptr);
}

#endif
