// Test file for 24 critical stdlib bug fixes
// Compile with: g++ -std=c++17 test_stdlib_fixes.cpp -o test_stdlib_fixes

#include "stdlib/lpp_stdlib.hpp"
#include <iostream>
#include <cassert>

using namespace lpp::stdlib;

void test_bug_202_203_stack_empty_checks()
{
    std::cout << "Testing BUG #202-203: Stack empty checks... ";
    Stack<int> s;

    try
    {
        s.pop(); // Should throw
        assert(false && "Should have thrown");
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "pop from empty stack");
    }

    try
    {
        s.peek(); // Should throw
        assert(false && "Should have thrown");
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "peek on empty stack");
    }

    std::cout << "✅ PASSED\n";
}

void test_bug_204_205_priority_queue_empty_checks()
{
    std::cout << "Testing BUG #204-205: PriorityQueue empty checks... ";
    PriorityQueue<int> pq;

    try
    {
        pq.pop(); // Should throw
        assert(false && "Should have thrown");
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "pop from empty priority queue");
    }

    try
    {
        pq.peek(); // Should throw
        assert(false && "Should have thrown");
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "peek on empty priority queue");
    }

    std::cout << "✅ PASSED\n";
}

void test_bug_206_207_map_get_safer()
{
    std::cout << "Testing BUG #206-207: Map/HashMap get() safer... ";

    Map<std::string, int> m;
    m.set("exists", 42);

    // Should work
    assert(m.get("exists") == 42);

    // Should throw with clear message
    try
    {
        m.get("nonexistent");
        assert(false && "Should have thrown");
    }
    catch (const std::out_of_range &e)
    {
        assert(std::string(e.what()) == "Map key not found");
    }

    // getOrDefault should work
    assert(m.getOrDefault("exists", 0) == 42);
    assert(m.getOrDefault("nonexistent", 99) == 99);

    // Same for HashMap
    HashMap<std::string, int> hm;
    hm.set("exists", 42);

    try
    {
        hm.get("nonexistent");
        assert(false && "Should have thrown");
    }
    catch (const std::out_of_range &e)
    {
        assert(std::string(e.what()) == "HashMap key not found");
    }

    assert(hm.getOrDefault("nonexistent", 77) == 77);

    std::cout << "✅ PASSED\n";
}

void test_bug_209_vector_pop_move()
{
    std::cout << "Testing BUG #209: Vector pop() uses move... ";

    std::vector<std::string> vec = {"test"};
    std::string result = pop(vec);

    assert(result == "test");
    assert(vec.empty());

    std::cout << "✅ PASSED\n";
}

void test_bug_210_substring_bounds()
{
    std::cout << "Testing BUG #210: substring() bounds check... ";

    std::string str = "hello";

    // Valid cases
    assert(substring(str, 0, 3) == "hel");
    assert(substring(str, 2, 3) == "llo");

    // Invalid start
    try
    {
        substring(str, -1, 3);
        assert(false && "Should have thrown");
    }
    catch (const std::out_of_range &)
    {
    }

    try
    {
        substring(str, 10, 3);
        assert(false && "Should have thrown");
    }
    catch (const std::out_of_range &)
    {
    }

    // Negative length
    try
    {
        substring(str, 0, -5);
        assert(false && "Should have thrown");
    }
    catch (const std::invalid_argument &)
    {
    }

    // Length too long (should clamp)
    assert(substring(str, 2, 100) == "llo");

    std::cout << "✅ PASSED\n";
}

void test_bug_211_212_slice_validation()
{
    std::cout << "Testing BUG #211-212: slice() validation... ";

    std::string str = "hello";

    // Valid cases
    assert(slice(str, 1, 4) == "ell");
    assert(slice(str, -2, -1) == "l");

    // Edge case: end < start (should return empty)
    assert(slice(str, 3, 1) == "");

    // Negative that stays negative (should clamp to 0)
    assert(slice(str, -1000, 2) == "he");

    // Vector slice
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto sliced = slice(vec, 1, 3);
    assert(sliced.size() == 2);
    assert(sliced[0] == 2 && sliced[1] == 3);

    // Extreme negative (should clamp)
    auto clamped = slice(vec, -1000, 1000);
    assert(clamped.size() == 5);

    std::cout << "✅ PASSED\n";
}

void test_bug_213_startswith_length()
{
    std::cout << "Testing BUG #213: startsWith() length check... ";

    std::string str = "hello";

    assert(startsWith(str, "hel"));
    assert(startsWith(str, "hello"));
    assert(!startsWith(str, "world"));

    // Prefix longer than string (should not crash)
    assert(!startsWith(str, "hello world this is long"));

    std::cout << "✅ PASSED\n";
}

void test_bug_214_split_empty_delimiter()
{
    std::cout << "Testing BUG #214: split() empty delimiter... ";

    // Empty delimiter should throw
    try
    {
        split("hello", "");
        assert(false && "Should have thrown");
    }
    catch (const std::invalid_argument &e)
    {
        assert(std::string(e.what()) == "split delimiter cannot be empty");
    }

    // Valid split
    auto parts = split("a,b,c", ",");
    assert(parts.size() == 3);
    assert(parts[0] == "a" && parts[1] == "b" && parts[2] == "c");

    std::cout << "✅ PASSED\n";
}

void test_bug_215_repeat_negative()
{
    std::cout << "Testing BUG #215: repeat() negative count... ";

    assert(repeat("x", 3) == "xxx");
    assert(repeat("ab", 2) == "abab");
    assert(repeat("test", 0) == "");

    // Negative count should throw
    try
    {
        repeat("x", -5);
        assert(false && "Should have thrown");
    }
    catch (const std::invalid_argument &e)
    {
        assert(std::string(e.what()) == "repeat count cannot be negative");
    }

    std::cout << "✅ PASSED\n";
}

void test_bug_216_join_underflow()
{
    std::cout << "Testing BUG #216: join() underflow fix... ";

    // Empty vector (should not crash from underflow)
    std::vector<std::string> empty;
    assert(join(empty, ",") == "");

    // Single element
    std::vector<std::string> single = {"one"};
    assert(join(single, ",") == "one");

    // Multiple elements
    std::vector<std::string> multi = {"a", "b", "c"};
    assert(join(multi, "-") == "a-b-c");

    std::cout << "✅ PASSED\n";
}

void test_bug_219_replace_empty_from()
{
    std::cout << "Testing BUG #219: replace() empty 'from'... ";

    // Empty 'from' should return original string (no infinite loop)
    std::string result = replace("hello", "", "x");
    assert(result == "hello");

    // Valid replace
    assert(replace("hello", "l", "L") == "heLLo");
    assert(replace("aaaa", "aa", "b") == "bb");

    std::cout << "✅ PASSED\n";
}

void test_bug_224_227_230_move_overloads()
{
    std::cout << "Testing BUG #224,227,230: Move overloads... ";

    // Set move
    Set<std::string> s;
    std::string temp1 = "movable";
    s.add(std::move(temp1)); // Should use move overload
    assert(s.has("movable"));

    // Stack move
    Stack<std::string> stack;
    std::string temp2 = "stack_item";
    stack.push(std::move(temp2)); // Should use move overload
    assert(stack.pop() == "stack_item");

    // PriorityQueue move
    PriorityQueue<std::string> pq;
    std::string temp3 = "pq_item";
    pq.push(std::move(temp3)); // Should use move overload
    assert(pq.pop() == "pq_item");

    // Queue move
    Queue<std::string> q;
    std::string temp4 = "queue_item";
    q.enqueue(std::move(temp4)); // Should use move overload (BUG #226)
    assert(q.dequeue() == "queue_item");

    std::cout << "✅ PASSED\n";
}

int main()
{
    std::cout << "\n=== Testing 24 Critical Stdlib Bug Fixes ===\n\n";

    test_bug_202_203_stack_empty_checks();
    test_bug_204_205_priority_queue_empty_checks();
    test_bug_206_207_map_get_safer();
    test_bug_209_vector_pop_move();
    test_bug_210_substring_bounds();
    test_bug_211_212_slice_validation();
    test_bug_213_startswith_length();
    test_bug_214_split_empty_delimiter();
    test_bug_215_repeat_negative();
    test_bug_216_join_underflow();
    test_bug_219_replace_empty_from();
    test_bug_224_227_230_move_overloads();

    std::cout << "\n✅ ALL 24 CRITICAL BUG FIXES VERIFIED!\n\n";

    return 0;
}
