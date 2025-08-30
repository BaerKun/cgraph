/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
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
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 */

// Modified by Bear Kun for cgraph.

#include "private/random.h"

#ifdef _MSC_VER // MSVC
#include <intrin.h>
typedef volatile int64_t atomic_int64_t;
#define atomic_add _InterlockedExchangeAdd64

#else
#include <stdatomic.h>
typedef _Atomic int64_t atomic_int64_t;
#define atomic_add atomic_fetch_add

#endif

CGraphRNG cgraphRngCreate(const uint64_t seed) {
  CGraphRNG rng;
  static atomic_int64_t seq = 0xda3e39cb94b95bdbULL;
  rng.inc = atomic_add(&seq, 0x853c49e6748fea9bULL) | 1;
  rng.state = seed + rng.inc;
  rng.state = rng.state * 6364136223846793005ULL + rng.inc;
  return rng;
}
