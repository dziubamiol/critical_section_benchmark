#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

struct Timer {
public:
    std::chrono::high_resolution_clock::time_point start_time;

    void start_timer() {
        start_time = std::chrono::steady_clock::now();
        std::cout << "started\n";
    }

    void get_time_elapsed() const {
        std::chrono::high_resolution_clock::time_point now = std::chrono::steady_clock::now();;
        std::chrono::duration<double> time_span =  now - start_time;

        std::cout << "finished after " << time_span.count() << "s\n";
    }

};

void some_cool_calculation(unsigned int &num) {
    for (unsigned int i = 0; i < 100000; i++) {
        num++;
    }
}

std::mutex mut;

void some_cool_mutex_calculation(unsigned int &num) {
    for (unsigned int i = 0; i < 10000000; i++) {
        mut.lock();
        num++;
        mut.unlock();
    }
}

void raw_calculation() {
    std::cout << "Raw calculation\n";
    unsigned int num = 0;
    Timer timer = Timer();
    std::vector<std::thread> threads;

    timer.start_timer();
    threads.emplace_back(std::thread(some_cool_calculation, std::ref(num)));
    threads.emplace_back(std::thread(some_cool_calculation, std::ref(num)));

    for (auto &thread : threads) {
        thread.join();
    }
    timer.get_time_elapsed();

    std::cout << "Num: " << num << "\n";
}

void mutex_calculation() {
    std::cout << "Mutex calculation\n";
    unsigned int num = 0;
    Timer timer = Timer();
    std::vector<std::thread> threads;

    timer.start_timer();
    threads.emplace_back(std::thread(some_cool_mutex_calculation, std::ref(num)));
    threads.emplace_back(std::thread(some_cool_mutex_calculation, std::ref(num)));

    for (auto &thread : threads) {
        thread.join();
    }
    timer.get_time_elapsed();

    std::cout << "Num: " << num << "\n";
}

int main() {
    raw_calculation();
    mutex_calculation();
    return 0;
}
