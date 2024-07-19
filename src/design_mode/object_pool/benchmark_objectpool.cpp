#include <benchmark/benchmark.h>
#include "object_pool.h"
#include <vector>
#include <string>
#include "buffer.h"

static void BM_NORMAL(benchmark::State& state) {
  std::string data("213313");
  for(auto _ : state) {
    std::vector<Buffer> vec;
    for(int i = 0; i < 20; i++) {
      vec.emplace_back(data.data(), data.size());
    }
  }
}

static void BM_OBJECT_POOL(benchmark::State& state) {
  std::string data("213313");
  auto func = []()-> std::unique_ptr<std::vector<Buffer>> {
    return std::make_unique<std::vector<Buffer>>(100);
  };
  ObjectPool<std::vector<Buffer>> object_pool(100, func);
  for(auto _ : state) {
    std::unique_ptr<std::vector<Buffer>> buf_ptr = object_pool.Get();
    for(int i = 0; i < 20; i++) {
      std::vector<Buffer> * buf = buf_ptr.get();
      (*buf)[i].Reset(data.data(), data.size());
    }
    object_pool.Set(std::move(buf_ptr));
  }
}

BENCHMARK(BM_OBJECT_POOL);
BENCHMARK(BM_NORMAL);

BENCHMARK_MAIN();
