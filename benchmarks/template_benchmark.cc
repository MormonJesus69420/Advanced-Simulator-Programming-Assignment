// google benchmark
#include <benchmark/benchmark.h>

// aspect - collision algorithms
#include <collision_algorithms.h>
using namespace simaspect;

#include <algorithm>
#include <limits>
#include <random>
#include <vector>

static void BM_benchfunction_name(benchmark::State& state)
{
  // The test loop
  int i{ 0 };
  while (state.KeepRunning())
    i += 1;
  benchmark::DoNotOptimize(i);
}
BENCHMARK(BM_benchfunction_name)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN()
