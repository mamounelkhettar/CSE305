#include <chrono>
#include <iostream>
#include <iterator>
#include <optional>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "td01.cpp"

template <typename Func>
void CheckParallelization(Func func) {
    // Finds the runtime for different num_threads values
    // Should improve until reaches the number of cores in your computer
    for (size_t num_threads = 1; num_threads < 16; ++num_threads) {
        auto start = std::chrono::steady_clock::now();
        func(num_threads);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Runtime for " << num_threads << " threads is ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    }
}

int main(int argc, char *argv[]) {

    std::cout << "Test for the summation code" << std::endl;

    // Simple correctness check
    std::vector<long double> from_one_to_hundred;
    for (size_t i = 0; i < 100; ++i) {
        from_one_to_hundred.push_back(i + 1);
    }
    /*
    // Should print 5050
    std::cout << SumParallel(from_one_to_hundred.begin(), from_one_to_hundred.end(), 2) << std::endl;

    std::uniform_real_distribution<long double> unif(0., 1.);
    std::default_random_engine re;
    std::vector<long double> test;
    for (size_t i = 0; i < 10000000; ++i) {
        test.push_back(unif(re));
    }
    CheckParallelization([&test](size_t nt) -> long double {return SumParallel(test.begin(), test.end(), nt);});

    //-----------------------------------------------------

    std::cout << "Test for the summation with function code" << std::endl;
    std::cout << SumParallel(from_one_to_hundred.begin(), from_one_to_hundred.end(), sqr, 2) << std::endl;
    CheckParallelization([&test](size_t nt) -> long double {return SumParallel(test.begin(), test.end(), sqr, nt);});

    //-----------------------------------------------------

    std::cout << "Test for the mean code" << std::endl;
    std::cout << MeanParallel(from_one_to_hundred.begin(), from_one_to_hundred.end(), 2) << std::endl;
    CheckParallelization([&test](size_t nt) -> long double {return MeanParallel(test.begin(), test.end(), nt);});

    //-----------------------------------------------------

    std::cout << "Test for the variance code" << std::endl;

    // Simple correctness check
    std::vector<long double> from_zero_to_nine;
    for (size_t i = 0; i < 10; ++i) {
        from_zero_to_nine.push_back(i);
    }
    // Should print 

    std::cout << VarianceParallel(from_zero_to_nine.begin(), from_zero_to_nine.end(), 2) << std::endl;

    CheckParallelization([&test](size_t nt) -> long double {return VarianceParallel(test.begin(), test.end(), nt);});
    */
    //-----------------------------------------------------
    
    std::cout << "Test for finding code" << std::endl;
    std::vector<int> test_find;
    for (size_t i = 0; i < 10000000; ++i) {
        test_find.push_back(rand() % 100);
    }

    CheckParallelization([&test_find](size_t nt) -> bool {return FindParallel(test_find.begin(), test_find.end(), 101, nt);});

    test_find[7000000] = 101;
    CheckParallelization([&test_find](size_t nt) -> bool {return FindParallel(test_find.begin(), test_find.end(), 101, nt);});
    
    //-------------------------------------------------------------------------

    std::cout << "Test for running with timeout" << std::endl;

    // should be too slow
    std::optional<int> result = RunWithTimeout<int, int>([](int a) {std::this_thread::sleep_for(std::chrono::milliseconds(200)); return 2 * a;}, 5, 100);
    if (result.has_value()) {
        std::cout << "On time! The result is " << result.value() << std::endl;
    } else {
        std::cout << "Too slooow" << std::endl;
    }

    // should be in time
    result = RunWithTimeout<int, int>([](int a) {std::this_thread::sleep_for(std::chrono::milliseconds(100)); return 2 * a;}, 5, 200);
    if (result.has_value()) {
        std::cout << "On time! The result is " << result.value() << std::endl;
    } else {
        std::cout << "Too slooow" << std::endl;
    }
    

}
