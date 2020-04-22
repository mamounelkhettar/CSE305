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

static Num sqr(Num x) {  return x*x ; } 

static void var(NumIter start, NumIter end, Num &x1, Num &x2, size_t num_threads) {
    x2 = std::accumulate(start, end, 0.) ;
    std::vector<Num> r;
    std::transform(start, end, std::back_inserter(r), sqr) ;
    x1 = std::accumulate(r.begin(), r.end(), 0.)  ;
}

static Num VarianceParallel(NumIter begin, NumIter end, size_t num_threads) {
    unsigned long const length=std::distance(begin,end); // distance between the two iterators
    if (!length) { return 0. ; } // if distance is 0
    
    unsigned long const block_size=length/num_threads;

    std::vector<Num> x1(num_threads);
    std::vector<Num> x2(num_threads);
    std::vector<std::thread> threads(num_threads-1);

    NumIter block_start=begin;
    for(unsigned long i=0; i<(num_threads-1); ++i)
    {
        NumIter block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(&var, block_start, block_end, std::ref(x1[i]), std::ref(x2[i]), num_threads) ;
        block_start = block_end;
    }
    var(block_start, end, std::ref(x1[num_threads-1]), std::ref(x2[num_threads-1]), num_threads); 
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    Num sum2 = std::accumulate(x2.begin(), x2.end(), 0.) / length ;
    return std::accumulate(x1.begin(), x1.end(), 0.)/length - sum2*sum2;
}

//-----------------------------------------------------------------------------
template <typename Iter, typename T>
static int search_value(Iter begin, Iter end, T target, bool &found) {
    for (Iter i = begin; i < end; i++) {
        if (found){
            return 0;
        }
        if(*i == target) {
            found = true ;
            return 0;
        }
    }
    return 0;
}

template <typename Iter, typename T>
static bool FindParallel(Iter begin, Iter end, T target, size_t num_threads) {
    unsigned long const length=std::distance(begin,end); // distance between the two iterators
    unsigned long const block_size = length/num_threads ;
    
    bool found = false;
    std::vector<std::thread> threads(num_threads-1);
    Iter block_start=begin;

    for(unsigned long i=0; i<(num_threads-1); ++i)
    {
        Iter block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(&search_value<Iter, T>, block_start, block_end, target, std::ref(found)) ;
        block_start = block_end;
    }
    search_value(block_start, end, target, std::ref(found)) ;
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return found ;
}

//-----------------------------------------------------------------------------
template <typename ReturnType>
std::optional<ReturnType> ret ;

template <typename ArgType, typename ReturnType>
static std::optional<ReturnType> check(ReturnType f(ArgType), ArgType arg) {
    ret<ReturnType> = f(arg) ;
}

template <typename ArgType, typename ReturnType>
std::optional<ReturnType> RunWithTimeout(ReturnType f(ArgType), ArgType arg, size_t timeout) {
    std::thread t(&check<ReturnType, ArgType>, f, arg) ;
    t.detach() ;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout)) ;
    return ret<ReturnType> ;
}

//-----------------------------------------------------------------------------
