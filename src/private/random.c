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
#include <stdatomic.h>

CGraphRNG cgraphRngCreate(const uint64_t seed) {
  static _Atomic uint64_t seq = 0xda3e39cb94b95bdbULL;
  CGraphRNG rng;
  rng.inc = atomic_fetch_add(&seq, 0x9e3779b97f4a7c15ULL) | 1u;
  rng.state = (seed ^ 0x853c49e6748fea9bULL) + rng.inc;
  rng.state = rng.state * 6364136223846793005ULL + rng.inc;
  return rng;
}
