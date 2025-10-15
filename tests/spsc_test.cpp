#include <iostream>
#include"lf_spsc.h"
#include <thread>
#include"l_spsc.h"

int PASS = 0;
int FAIL = 0;

template<typename T>
void run_test_1(T& q){
    //SPSCQueue<int> q(4);
    std::vector<bool> ans;
    std::vector<int> vals;
    ans.push_back(q.push(1));
    ans.push_back(q.push(2));
    int x;
    ans.push_back(q.pop(x));
    vals.push_back(x);
    ans.push_back(q.pop(x));
    vals.push_back(x);
    ans.push_back(q.pop(x));
    vals.push_back(x);
    std::vector<bool> expected_ans({true, true, true, true, false});
    std::vector<int> expected_vals({1, 2, 2});
    if(ans ==  expected_ans && vals == expected_vals){
        std::cout << "Test 1: PASS" << '\n';
        PASS++;
    }
    else{
        std::cout << "Test 1: FAIL" << '\n';
        FAIL++;
    }
}

template<typename T>
void run_test_2(T& q){
    //SPSCQueue<int> q(4);
    std::vector<bool> ans;
    ans.push_back(q.push(1));
    ans.push_back(q.push(2));
    ans.push_back(q.push(3));
    ans.push_back(q.push(4));
    ans.push_back(q.push(5));
    std::vector<bool> expected_ans({true, true, true, true, false});
    std::cout << "Test 2: ";
    if(ans ==  expected_ans){
        std::cout << "PASS" << '\n';
        PASS++;
    }
    else{
        std::cout << "FAIL" << '\n';
        FAIL++;
    }
}

template<typename T>
void run_test_3(T& q){
    //SPSCQueue<int> q(4);
    std::cout << "TEST 3: ";
    int x;
    if(q.pop(x) == false){
        std::cout << "PASS" << '\n';
        PASS++;
    }
    else{
        std::cout << "FAIL" << '\n';
        FAIL++;
    }
}

template<typename T>
void produce(T &q){
    for(int i=0;i<1e7;i++){
        q.push(i);
    }
}

template<typename T>
void consume(T &q, std::atomic<bool> &producerFinished){
    int prev=-1;
    int x;
    bool popped;
    bool fail = false;
    while(!producerFinished || !q.empty()){
        popped = q.pop(x);
        //std::cout << popped << " " << x << '\n';
        if(!popped){
            continue;
        }
        if(x <= prev){
            fail = true;
            break;
        }
        prev = x;
    }
    std::cout << "Test 4: ";
    if(!fail){
        std::cout << "PASS" << '\n';
        PASS++;
    }
    else{
        std::cout << "FAIL" << '\n';
        FAIL++;
    }
}

template<typename T>
void run_test_4(T& q){
    //SPSCQueue<int> q(1e6);
    std::atomic<bool> producerFinished(false);
    std::thread producer(produce, std::ref(q));
    std::thread consumer(consume, std::ref(q), std::ref(producerFinished));
    
    producer.join();
    producerFinished.store(true);

    consumer.join();
}

void test_locked(){
    std::cout << "Testing locked queue" << '\n';
    locked::SPSCQueue<int> q1(4);
    run_test_1(q1);
    locked::SPSCQueue<int> q2(4);
    run_test_2(q2);
    locked::SPSCQueue<int> q3(4);
    run_test_3(q3);
    locked::SPSCQueue<int> q4(1e6);
    run_test_3(q4);
}
void test_lock_free(){
    std::cout << "Testing lock free queue" << '\n';
    lf::SPSCQueue<int> q1(4);
    run_test_1(q1);
    lf::SPSCQueue<int> q2(4);
    run_test_2(q2);
    lf::SPSCQueue<int> q3(4);
    run_test_3(q3);
    lf::SPSCQueue<int> q4(1e6);
    run_test_3(q4);
}

int main(){
    test_locked();
    test_lock_free();
    std::cout << "Total passes: " << PASS << "\nTotal fails: " << FAIL << '\n';
}
