#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

template<typename T>
std::pair<T, std::chrono::duration<double>> parallel_sum_std(T a[], size_t N, size_t num_threads) {
    std::atomic<T> sum(0);
    std::vector<std::thread> threads;
    std::mutex mtx;

    // Calculate the number of elements each thread should process
    size_t elements_per_thread = N / num_threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread([&, i] {
            T local_sum = 0;
            size_t start_index = i * elements_per_thread;
            size_t end_index = (i + 1) * elements_per_thread;

            //The last thread processes all remaining elements
            if (i == num_threads - 1) {
                end_index = N;
            }

            for (size_t j = start_index; j < end_index; ++j) {
                local_sum += a[j];
            }

            // Use a mutex lock to ensure atomicity of the operation
            std::lock_guard<std::mutex> guard(mtx);
            sum += local_sum;
        }));
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    return {sum.load(), duration};
}

int main() {
    size_t N = 1000000; // array size
    size_t num_threads = 4; // Number of threads used
    std::vector<int> data(N, 1); // Initialize the array with each element being 1

    auto result = parallel_sum_std(data.data(), data.size(), num_threads);

    std::cout << "Sum: " << result.first << std::endl;
    std::cout << "Time taken: " << result.second.count() << " seconds" << std::endl;

    return 0;

//int main() {
//    size_t N = 2000; // 数组大小
//    std::vector<int> data(N, 1); // 初始化数组，每个元素为1
//
//    for (size_t num_threads = 1; num_threads <= 16; ++num_threads) {
//        auto result = parallel_sum_std(data.data(), data.size(), num_threads);
//
//        std::cout << "Threads: " << num_threads << " Sum: " << result.first
//                  << " Time taken: " << result.second.count() << " seconds" << std::endl;
//    }
//
//    return 0;

//    int main() {
//        for (int test = 0; test < 10; ++test) {
//            size_t N = 1000 * (test + 1); // 数组大小从1000开始，每次增加1000
//            size_t num_threads = 1 + test; // 线程数从1开始，每次增加1
//            std::vector<int> data(N, 1); // 初始化数组，每个元素为1
//
//            auto result = parallel_sum_std(data.data(), data.size(), num_threads);
//
//            std::cout << "Test " << test + 1 << ": N = " << N << ", Threads = " << num_threads
//                      << ", Sum = " << result.first << ", Time taken = " << result.second.count()
//                      << " seconds" << std::endl;
//        }
//
//        return 0;
}
