#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

template<typename T>
std::pair<T, std::chrono::duration<double>> parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads) {
    T sum = 0;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Specify num_threads and use OpenMP's built-in reduction operations
#pragma omp parallel for reduction(+:sum) num_threads(num_threads)
    for (size_t i = 0; i < N; ++i) {
        sum += a[i];
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    return {sum, duration};
}

int main() {
    size_t N = 1000000; // array size
    size_t num_threads = 4; // Number of threads used
    std::vector<int> data(N, 1); // Initialize the array with each element being 1

    auto result = parallel_sum_omp_builtin(data.data(), data.size(), num_threads);

    std::cout << "Sum: " << result.first << std::endl;
    std::cout << "Time taken: " << result.second.count() << " seconds" << std::endl;

    return 0;


//int main() {
//    size_t N = 2000; // 修改数组大小为1000
//    std::vector<int> data(N, 1); // 初始化数组，每个元素为1
//
//    // 循环遍历线程数从1到16
//    for (size_t num_threads = 1; num_threads <= 16; ++num_threads) {
//        auto result = parallel_sum_omp_builtin(data.data(), data.size(), num_threads);
//
//        std::cout << "Threads: " << num_threads << ", Sum: " << result.first
//                  << ", Time taken: " << result.second.count() << " seconds" << std::endl;
//    }
//
//    return 0;

//int main() {
//    // 循环10次测试，每次递增N和num_threads
//    for (int test = 1; test <= 10; ++test) {
//        size_t N = 1000 * test; // 数组大小从1000开始，每次增加1000
//        size_t num_threads = test; // 线程数从1开始，每次增加1
//        std::vector<int> data(N, 1); // 初始化数组，每个元素为1
//
//        auto result = parallel_sum_omp_builtin(data.data(), data.size(), num_threads);
//
//        std::cout << "Test " << test << ": N = " << N << ", Threads = " << num_threads
//                  << ", Sum = " << result.first << ", Time taken = " << result.second.count()
//                  << " seconds" << std::endl;
//    }
//
//    return 0;
}
