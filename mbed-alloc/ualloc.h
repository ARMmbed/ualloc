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

typedef struct uAllocTraitsExt UAllocTraitsExt_t;

typedef union uAllocTraits {
    struct {
        unsigned unused:26;
        unsigned align:2; // 4, 8, 16, or 32-byte alignment
        unsigned onChip:1;
        unsigned nonRetained:1;
        unsigned neverFree:1;
        unsigned reserved:1;
    } flags;
    UAllocTraitsExt_t *extended;
} UAllocTraits_t;

#if sizeof(UAllocTraits_t) > sizeof(uintptr_t)
#error UAllocTraits must be no larger than a uintptr_t
#endif

void * uAlloc(size_t bytes, UAllocTraits_t);
void uFree(void * ptr);

#endif // __MBED_ALLOC_UALLOC_H
