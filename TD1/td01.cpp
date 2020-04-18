#include <iostream>
#include <thread>
#include <numeric>
#include <iterator>
#include <optional>
#include <vector>
#include <algorithm>

typedef long double Num;
typedef std::vector<long double>::const_iterator NumIter;

//-----------------------------------------------------------------------------
static void accumulate(NumIter start, NumIter end, Num &sum) {
    sum = std::accumulate(start, end, 0.) ;
}
static Num SumParallel(NumIter begin, NumIter end, size_t num_threads) {
    
    unsigned long const length=std::distance(begin,end); // distance between the two iterators
    if (!length) { return 0. ; } // if distance is 0 

    unsigned long const block_size=length/num_threads;

    std::vector<Num> results(num_threads);
    std::vector<std::thread> threads(num_threads-1);

    NumIter block_start=begin;
    for(unsigned long i=0; i<(num_threads-1); ++i)
    {
        NumIter block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate, block_start, block_end, std::ref(results[i])) ;
        block_start = block_end;
    }
    accumulate(block_start, end, results[num_threads-1]); 
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(),results.end(), 0.);
}

template <typename Func>
static Num SumParallel(NumIter begin, NumIter end, Func f, size_t num_threads) {}

//-----------------------------------------------------------------------------

static Num MeanParallel(NumIter begin, NumIter end, size_t num_threads) {}

//-----------------------------------------------------------------------------

static Num VarianceParallel(NumIter begin, NumIter end, size_t num_threads) {}

//-----------------------------------------------------------------------------

template <typename Iter, typename T>
bool FindParallel(Iter begin, Iter end, T target, size_t num_threads) {}

//-----------------------------------------------------------------------------

template <typename ArgType, typename ReturnType>
std::optional<ReturnType> RunWithTimeout(ReturnType f(ArgType), ArgType arg, size_t timeout) {}

//-----------------------------------------------------------------------------
