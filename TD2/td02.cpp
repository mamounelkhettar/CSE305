#include <iostream>
#include <thread>
#include <numeric>
#include <iterator>
#include <optional>
#include <vector>
#include <algorithm>
#include <functional>

template <typename T, typename Iter>
static void max(Iter begin, Iter end, size_t chunk_size, T &Max){ 
    size_t length=std::distance(begin,end); 
   // base case
   if (length <= chunk_size) {
        T max = *std::max_element(begin, end) ;
        if (max > Max) {
            Max = max ;
        }
    } else {
        Iter mid = begin+(end-begin)/2 ;
        std::thread z(&max<T, Iter>, begin, mid, chunk_size, std::ref(Max)) ;
        std::thread w(&max<T, Iter>, mid, end, chunk_size, std::ref(Max)) ;
        z.join() ;
        w.join() ;
    }
    
}
template <typename T, typename Iter>
static T ParallelMax(Iter begin, Iter end, size_t chunk_size) {
    size_t length=std::distance(begin,end); // distance between the two iterators

    T Max = *begin;
    // base case 
    if (length <= chunk_size) {
        Max = *std::max_element(begin, end) ;
        return Max;
    } else {
        Iter mid = begin+(end-begin)/2 ;
        std::thread x(&max<T, Iter>, begin, mid, chunk_size, std::ref(Max)) ;
        std::thread y(&max<T, Iter>, mid, end, chunk_size, std::ref(Max)) ;
        x.join() ;
        y.join() ;
        return Max ;
    }
}

template <typename Iter>
void ParallelMerge(Iter lbegin, Iter lend, Iter rbegin, Iter rend, Iter dest, size_t num_threads) {}
