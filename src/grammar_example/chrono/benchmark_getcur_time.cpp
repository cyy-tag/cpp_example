#include <benchmark/benchmark.h>
#include "get_curr_time.h"

static void BM_CGetMicroTime(benchmark::State& state) {
  for(auto _ : state) {
    GetMicroTime();
  }
}

static void BM_ChronoyGetMicroTime(benchmark::State& state) {
  for(auto _ : state) {
    GetCurretTime();
  }
}

BENCHMARK(BM_CGetMicroTime);
BENCHMARK(BM_ChronoyGetMicroTime);

BENCHMARK_MAIN();
