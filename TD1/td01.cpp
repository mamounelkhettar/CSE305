#include <iostream>
#include <thread>
#include <numeric>
#include <iterator>
#include <optional>
#include <vector>
#include <algorithm>
#include <functional>

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
static void accumulate_f(NumIter start, NumIter end, Func f, Num &sum) { 
    std::vector<Num> r;
    std::transform(start, end, std::back_inserter(r), f) ;
    sum = std::accumulate(r.begin(), r.end(), 0.) ;
}

template <typename Func>
static Num SumParallel(NumIter begin, NumIter end, Func f, size_t num_threads) {
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
        threads[i] = std::thread(&accumulate_f<Func>, block_start, block_end, f, std::ref(results[i])) ;
        block_start = block_end;
    }
    accumulate_f(block_start, end, f, results[num_threads-1]); 
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(),results.end(), 0.);
}

//-----------------------------------------------------------------------------

static Num MeanParallel(NumIter begin, NumIter end, size_t num_threads) {
    unsigned long const length=std::distance(begin,end); // distance between the two iterators
    if (!length) { return 0. ; } // if distance is 0
    return SumParallel(begin, end, num_threads)/length ;
}

//-----------------------------------------------------------------------------

static Num srq(Num x) {
    return x*x;
}
static Num VarianceParallel(NumIter begin, NumIter end, size_t num_threads) {
    unsigned long const length=std::distance(begin,end); // distance between the two iterators
    if (!length) { return 0. ; } // if distance is 0
 

    Num x = SumParallel(begin, end, srq, num_threads) / length ; // E(X^2)
    Num x1 = srq(MeanParallel(begin, end, num_threads)) ; //(E(X))

    return x - (x1*x1) ;
}

//-----------------------------------------------------------------------------

template <typename Iter, typename T>
bool FindParallel(Iter begin, Iter end, T target, size_t num_threads) {}

//-----------------------------------------------------------------------------

template <typename ArgType, typename ReturnType>
std::optional<ReturnType> RunWithTimeout(ReturnType f(ArgType), ArgType arg, size_t timeout) {}

//-----------------------------------------------------------------------------
