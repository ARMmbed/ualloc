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
#ifndef __MBED_ALLOC_H
#define __MBED_ALLOC_H

#include <unistd.h>

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

#ifndef HEAP_SIZE
extern uint32_t __heap_size;
// Extract linker heap size parameter
#define HEAP_SIZE ((ptrdiff_t) &__heap_size)
#endif
extern uint32_t KRBS_START;
extern uint32_t SBRK_START;

#ifdef __cplusplus
extern "C" {
#endif
caddr_t _sbrk(const ptrdiff_t size);
void * krbs(const ptrdiff_t size);
void * krbs_ex(const ptrdiff_t size, ptrdiff_t *actual);
#ifdef __cplusplus
}
#endif


#endif // __MBED_ALLOC_H
