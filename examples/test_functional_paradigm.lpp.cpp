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

int factorial(int n) {
    if ((n == 0)) {
        return 1;
    }
    return (n * factorial((n - 1)));
}
int double(int x) {
    return (x * 2);
}
int square(int x) {
    return (x * x);
}
bool is_even(int x) {
    return ((x % 2) == 0);
}
int sum(int a, int b) {
    return (a + b);
}
int main() {
    auto result = factorial(5);
    auto nums = std::vector<auto>{1, 2, 3, 4, 5};
    auto doubled = ([&]() { std::vector<decltype([](auto x) { return double(x); }(std::declval<decltype(nums)::value_type>()))> __result; for (auto& __item : nums) { __result.push_back(([](auto x) { return double(x); })(__item)); } return __result; })();
    auto squared = ([&]() { std::vector<decltype([](auto x) { return square(x); }(std::declval<decltype(nums)::value_type>()))> __result; for (auto& __item : nums) { __result.push_back(([](auto x) { return square(x); })(__item)); } return __result; })();
    return 0;
}
