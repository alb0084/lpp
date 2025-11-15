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

std::string dayName(int day) {
    switch (day) {
        case 1:
            return "Monday";
            break;
        case 2:
            return "Tuesday";
            break;
        case 3:
            return "Wednesday";
            break;
        case 4:
            return "Thursday";
            break;
        case 5:
            return "Friday";
            break;
        case 6:
            return "Saturday";
            break;
        case 7:
            return "Sunday";
            break;
        default:
            return "Invalid day";
            break;
    }
}
std::string gradeToLetter(int score) {
    std::string letter = "";
    switch (score) {
        case 100:
        case 90:
            letter = "A";
            break;
        case 80:
            letter = "B";
            break;
        case 70:
            letter = "C";
            break;
        case 60:
            letter = "D";
            break;
        default:
            letter = "F";
            break;
    }
    return letter;
}
int testFallThrough(int x) {
    int result = 0;
    switch (x) {
        case 1:
            result = (result + 1);
        case 2:
            result = (result + 2);
        case 3:
            result = (result + 3);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}
int main() {
    std::string day = dayName(3);
    std::string grade = gradeToLetter(90);
    int fall = testFallThrough(1);
    return 0;
}
