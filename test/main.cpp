#include <iostream>
#include <thread>
#include <numeric>
#include <iterator>
#include <optional>
#include <vector>
#include <algorithm>
#include <functional>

void w(int& x) {
    while (x != 2) {}
    x = 2 * x;
}

void wait2(int& x) {
    while (x != 3) {}
    x = 2 * x;
}

void inc(int& x) {
    x = x + 1;
}

int main() {
    int x = 1;
    
    std::thread a = std::thread(&w, std::ref(x)) ;
    std::thread b(&wait2, std::ref(x));
    std::thread c(&inc, std::ref(x));
    a.join();
    b.detach();
    c.detach();
    std::cout << x << std::endl;
}