#include"lf_spsc.h"

#include <benchmark/benchmark.h>
#include <thread>

std::atomic<bool> PRODUCER_FINISHED(false);

void produce_lf(lf::SPSCQueue<int> &q, int push_el){
    for(int i=0;i<push_el;i++)
        q.push(i);
    PRODUCER_FINISHED.store(true);
}

void consume_lf(lf::SPSCQueue<int> &q){
    int x;
    while(!q.empty() && PRODUCER_FINISHED){
        q.pop(x);
    }
}

void lockfree_task(int push_el){
    auto q = lf::SPSCQueue<int>(1e3);
    auto producer = std::thread(produce_lf, std::ref(q), push_el);
    auto consumer = std::thread(consume_lf, std::ref(q));
    producer.join();
    consumer.join();
}

static void BM_SPSC(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    lockfree_task(state.range(0));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SPSC)-> RangeMultiplier(10) -> Range(1e3, 1e7);
// Run the benchmark
BENCHMARK_MAIN();
