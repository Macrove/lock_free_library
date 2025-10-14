#include <atomic>
#include <cassert>
#include <cstddef>
#include <vector>
#include <Utility.h>

namespace lf{


template <typename T>
class SPSCQueue{
public:
    explicit SPSCQueue(size_t capacity_)
    {
        capacity = ceil_pow_of_2(capacity_);
        q.resize(capacity);
    }

    bool push(const T& item){
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire);

        if(t - h == capacity) return false;

        q[get_idx(t)] = item;
        tail.store(t+1, std::memory_order_release);

        return true;
    }
    bool pop(T& item){
        size_t h = head.load(std::memory_order_relaxed);
        size_t t = tail.load(std::memory_order_acquire);
        
        if(t == h) return false;

        item = q[get_idx(h)]; head.store(h+1, std::memory_order_release);

        return true;
    }
    inline bool empty() const{
        size_t h = head.load(std::memory_order_acquire);
        size_t t = tail.load(std::memory_order_acquire);
        return h==t;
    }
    inline bool full() const {
        size_t h = head.load(std::memory_order_acquire);
        size_t t = tail.load(std::memory_order_acquire);
        return t - h == capacity;
    }

private:
    inline size_t get_idx(size_t v) const {
        return v & (capacity - 1);
    }

    size_t capacity;
    alignas(CACHE_LINE_SIZE) std::atomic<size_t> head{0};
    alignas(CACHE_LINE_SIZE) std::atomic<size_t> tail{0};
    std::vector<T> q;
};

}
