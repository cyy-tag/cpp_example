#include <benchmark/benchmark.h>
#include "get_object.h"


thread_local Test ThreadLocalConcurrence::test_{};

static void BM_VectorCon(benchmark::State& state) {
  Concurrence con;
  for(auto _ : state) {
    con.Run(20);
  }
}

static void BM_ThreadLocalCon(benchmark::State& state) {
  ThreadLocalConcurrence con;
  for(auto _ : state) {
    con.Run(20);
  }
}

BENCHMARK(BM_VectorCon);
BENCHMARK(BM_ThreadLocalCon);

BENCHMARK_MAIN();
