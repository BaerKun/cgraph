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

#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdatomic.h>

typedef struct {
  uint64_t state;
  uint64_t inc;
} CGraphRNG;

static inline uint32_t random_(CGraphRNG *rng) {
  const uint64_t oldstate = rng->state;
  rng->state = oldstate * 6364136223846793005ULL + rng->inc;
  const uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  const uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static inline CGraphRNG cgraphRngCreate(const uint64_t seed) {
  static _Atomic uint64_t seq = 0xda3e39cb94b95bdbULL;
  CGraphRNG rng;
  rng.inc = atomic_fetch_add(&seq, 0xa7a83fd083e2f5b2ULL) | 1u;
  rng.state = (seed ^ 0x853c49e6748fea9bULL) + rng.inc;
  rng.state = rng.state * 6364136223846793005ULL + rng.inc;
  return rng;
}

static inline uint32_t cgraphRandomInt(CGraphRNG *rng, const uint32_t bound) {
  return ((uint64_t)random_(rng) * (uint64_t)bound) >> 32;
}

static inline float cgraphRandomFloat(CGraphRNG *rng, const float bound) {
  return (float)(random_(rng) >> 1) * bound / 2147483647.f;
}

#endif // RANDOM_H