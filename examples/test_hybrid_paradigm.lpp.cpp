#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <functional>
#include <variant>
#include <map>
#include <any>

void print(const std::string& s) {
    std::cout << s << std::endl;
}

void print(int n) {
    std::cout << n << std::endl;
}

void print(double n) {
    std::cout << n << std::endl;
}

// Higher-order function: map
template<typename T, typename F>
auto map(const std::vector<T>& vec, F func) {
    std::vector<decltype(func(vec[0]))> result;
    for (const auto& item : vec) {
        result.push_back(func(item));
    }
    return result;
}

// Higher-order function: filter
template<typename T, typename F>
std::vector<T> filter(const std::vector<T>& vec, F predicate) {
    std::vector<T> result;
    for (const auto& item : vec) {
        if (predicate(item)) {
            result.push_back(item);
        }
    }
    return result;
}

// Higher-order function: reduce/fold
template<typename T, typename F>
T reduce(const std::vector<T>& vec, T init, F func) {
    T result = init;
    for (const auto& item : vec) {
        result = func(result, item);
    }
    return result;
}

int double(int x) {
    return (x * 2);
}
int triple(int x) {
    return (x * 3);
}
int imperative_sum() {
    mut auto sum = 0;
    mut auto i = 1;
    while ((i <= 5)) {
        sum = (sum + i);
        i = (i + 1);
    }
    return sum;
}
int functional_factorial(int n) {
    if ((n <= 1)) {
        return 1;
    }
    return (n * functional_factorial((n - 1)));
}
int main() {
    auto total = imperative_sum();
    auto fact = functional_factorial(5);
    auto doubled = double(total);
    auto tripled = triple(fact);
    return 0;
}
