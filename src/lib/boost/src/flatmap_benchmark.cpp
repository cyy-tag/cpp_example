#include <benchmark/benchmark.h>
#include <boost/container/flat_map.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <unordered_map>
#include <map>

using namespace boost::container;
const size_t num_elements = 1000;

static void BM_FLAT_MAP_SEARCH(benchmark::State& state)
{
  flat_map<int, int> map;
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(0, num_elements);
  for (size_t i = 0; i < num_elements; ++i) {
      map.emplace(dist(rng), static_cast<int>(i));
  }
  for(auto _ : state)
  {
    map[dist(rng)];
  }
}

static void BM_UNORDERED_MAP_SEARCH(benchmark::State& state)
{
  std::unordered_map<int,int> map;
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(0, num_elements);
  for (size_t i = 0; i < num_elements; ++i) {
      map.emplace(dist(rng), static_cast<int>(i));
  }
  for(auto _ : state)
  {
    map.find(dist(rng));
  }
}

static void BM_MAP_SEARCH(benchmark::State& state)
{
  std::map<int,int> map_;
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(0, num_elements);
  for (size_t i = 0; i < num_elements; ++i) {
      map_.emplace(dist(rng), static_cast<int>(i));
  }
  for(auto _ : state)
  {
    map_.find(dist(rng));
  }
}

BENCHMARK(BM_UNORDERED_MAP_SEARCH);
BENCHMARK(BM_MAP_SEARCH);
BENCHMARK(BM_FLAT_MAP_SEARCH);

BENCHMARK_MAIN();