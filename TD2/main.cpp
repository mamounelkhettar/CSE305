#include <chrono>
#include <iostream>
#include <vector>
#include "td02.cpp"


int main() {
    // ParallelMax 
    std::vector<int> v;
    for (size_t i = 0; i < 20; ++i) {
        v.push_back(rand() % 1000);
    }
    for (size_t i = 0; i < 20; ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << ParallelMax<int, std::vector<int>::iterator>(v.begin(), v.end(), 5) << std::endl;
    /*
    // ParallelMerge
    std::vector<int> left;
    std::vector<int> right;
    for (size_t i = 0; i < 20; ++i) {
        left.push_back(rand() % 1000);
        right.push_back(rand() % 1000);
    }
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<int> dest(40);
    ParallelMerge(left.begin(), left.end(), right.begin(), right.end(), dest.begin(), 3);
    std::cout << std::is_sorted(dest.begin(), dest.end()) << std::endl;

    for (size_t nt = 1; nt < 10; ++nt) {
        std::vector<int> L;
        std::vector<int> R;
        for (size_t i = 0; i < 1000000; ++i) {
            L.push_back(rand() % 100000);
            R.push_back(rand() % 100000);
        }
        std::sort(L.begin(), L.end());
        std::sort(R.begin(), R.end());
        std::vector<int> D(2000000);
        auto start = std::chrono::steady_clock::now(); 
        ParallelMerge(L.begin(), L.end(), R.begin(), R.end(), D.begin(), nt);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Runtime for " << nt << " threads is ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl; 
        std::cout << std::is_sorted(dest.begin(), dest.end()) << std::endl;  
    }
    */
    return 0;
}
