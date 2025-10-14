#include <iostream>
#include <mutex>
#include <vector>
#include<stddef.h>
#include"Utility.h"

namespace locked{

template<typename T>
class SPSCQueue{
public:
    SPSCQueue(size_t capacity_){
        capacity = ceil_pow_of_2(capacity_);
        q.resize(capacity);
        head = 0;
        tail = 0;
    }
    bool push(const T& item){
        std::cout << tail << " " << head << " " << item << '\n';
        std::scoped_lock<std::mutex> scop_lock(mtx);
        if(tail - head == capacity){
            return false;
        }
        q[get_idx(tail)] = item;
        tail++;
        return true;
    }
    bool pop(T& item){
        std::scoped_lock<std::mutex> scop_lock(mtx);
        if(tail == head){
            return false;
        }
        item = q[get_idx(head)];
        head++;
        return true;
    }
    inline bool empty() const{
        std::scoped_lock<std::mutex> scop_lock(mtx);
        return tail == head;
    }
    inline bool full() const{
        std::scoped_lock<std::mutex> scop_lock(mtx);
        return tail - head == capacity;

    }

private:
    size_t capacity;
    mutable std::mutex mtx;
    std::vector<T> q;
    size_t head;
    size_t tail;
    inline size_t get_idx(size_t v) const {
        return v & (capacity - 1);
    }
};
}
