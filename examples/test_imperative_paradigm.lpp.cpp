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

int sum_five_numbers() {
    mut auto sum = 0;
    mut auto i = 0;
    while ((i < 5)) {
        sum = (sum + i);
        i = (i + 1);
    }
    return sum;
}
int find_max_of_five() {
    auto a = 5;
    auto b = 2;
    auto c = 8;
    auto d = 1;
    auto e = 9;
    mut auto max = a;
    if ((b > max)) {
        max = b;
    }
    if ((c > max)) {
        max = c;
    }
    if ((d > max)) {
        max = d;
    }
    if ((e > max)) {
        max = e;
    }
    return max;
}
int count_to_ten() {
    mut auto count = 0;
    mut auto i = 1;
    while ((i <= 10)) {
        count = (count + 1);
        i = (i + 1);
    }
    return count;
}
int main() {
    auto total = sum_five_numbers();
    auto maximum = find_max_of_five();
    auto counter = count_to_ten();
    return 0;
}
