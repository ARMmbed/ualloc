/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MBED_ALLOC_UALLOC_H
#define __MBED_ALLOC_UALLOC_H

typedef struct UAllocTraitsExt UAllocTraitsExt_t;

#define UALLOC_TRAITS_BITMASK        1
#define UALLOC_TRAITS_NEVER_FREE    (1<<1 | (BITMASK))
#define UALLOC_TRAITS_ZERO_FILL     (1<<2 | (BITMASK))
#define UALLOC_TRAITS_NON_RETAINED  (1<<3 | (BITMASK))
#define UALLOC_TRAITS_ON_CHIP       (1<<4 | (BITMASK))
#define UALLOC_TRAITS_ALIGN_4       (0<<5 | (BITMASK))
#define UALLOC_TRAITS_ALIGN_8       (1<<5 | (BITMASK))
#define UALLOC_TRAITS_ALIGN_16      (2<<5 | (BITMASK))
#define UALLOC_TRAITS_ALIGN_32      (3<<5 | (BITMASK))

typedef union UAllocTraits {
    uint32_t flags;
    UAllocTraitsExt_t *extended;
} UAllocTraits_t;

#if sizeof(UAllocTraits_t) > sizeof(uintptr_t)
#error UAllocTraits must be no larger than a uintptr_t
#endif

void * ualloc(size_t bytes, UAllocTraits_t);
void * urealloc(void * ptr, size_t bytes, UAllocTraits_t);
void ufree(void * ptr);

#endif // __MBED_ALLOC_UALLOC_H
