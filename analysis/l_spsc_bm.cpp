#include "Utility.h"
#include "l_spsc.h"

#include <atomic>
#include <benchmark/benchmark.h>
#include <thread>

std::atomic<bool> PRODUCER_FINISHED(false);

void produce_l(locked::SPSCQueue<int> &q, int push_el){
    for(int i=0;i<push_el;i++)
        while(!q.push(i)){}
    PRODUCER_FINISHED.store(true, std::memory_order_release);
}

void consume_l(locked::SPSCQueue<int> &q){
    int x;
    while(!PRODUCER_FINISHED.load(std::memory_order_relaxed)){
        while (q.pop(x)){}
    }
}

void locked_push_and_pop_bm(int push_el){
    PRODUCER_FINISHED.store(false, std::memory_order_acquire);
    auto q = locked::SPSCQueue<int>(1e5);
    auto producer = std::thread(produce_l, std::ref(q), push_el);
    auto consumer = std::thread(consume_l, std::ref(q));
    pin_thread_to_core(producer, 2);
    pin_thread_to_core(consumer, 3);
    producer.join();
    consumer.join();
}
void locked_push_bm(int push_el){
    PRODUCER_FINISHED.store(false, std::memory_order_acquire);
    auto q = locked::SPSCQueue<int>(1e4);
    auto producer = std::thread(produce_l, std::ref(q), push_el);
    pin_thread_to_core(producer, 2);
    producer.join();
}

void locked_pop_bm(){
    auto q = locked::SPSCQueue<int>(1e4);
    auto consumer = std::thread(consume_l, std::ref(q));
    pin_thread_to_core(consumer, 3);
    consumer.join();
}

static void BM_PUSH_POP_SPSC(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    locked_push_and_pop_bm(state.range(0));
  }
}

static void BM_PUSH_THEN_POP_SPSC(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    locked_push_bm(state.range(0));
    locked_pop_bm();
  }
}

// Register the function as a benchmark
BENCHMARK(BM_PUSH_POP_SPSC)-> RangeMultiplier(10) -> Range(1e1, 1e9);
//BENCHMARK(BM_PUSH_THEN_POP_SPSC)-> RangeMultiplier(10) -> Range(1e3, 1e7);
// Run the benchmark
BENCHMARK_MAIN();
