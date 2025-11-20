# Test suite for 24 critical stdlib bug fixes

add_executable(test_stdlib_fixes test_stdlib_fixes.cpp)
target_include_directories(test_stdlib_fixes PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_features(test_stdlib_fixes PRIVATE cxx_std_17)
