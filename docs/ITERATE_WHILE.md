# Iterate-While Operators

LPP includes Haskell-inspired operators for generating sequences by iteration with predicates.

## Overview

Three operator variants for different iteration patterns:

| Operator | Syntax | Description |
|----------|--------|-------------|
| `!!<` | `start !!< limit` | Auto-increment until limit |
| `!!>` | `start !!> limit` | Auto-decrement until limit |
| `!! $` | `start !! cond $ step` | Iterate while condition with custom step |
| `~> !!` | `start ~> step !! cond` | Alternative syntax (step-first) |

## Auto-Iterate Operators

### `!!<` — Auto-Increment Until

Generates ascending sequence with automatic increment.

**Syntax:**
```lpp
start !!< limit
```

**Examples:**
```lpp
let nums = 0 !!< 10;      // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
let range = 5 !!< 15;     // [5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
let single = 100 !!< 101; // [100]
```

**Transpiled C++:**
```cpp
([&]() { 
    std::vector<int> __result; 
    int __current = 0; 
    int __limit = 10; 
    while (__current < __limit) { 
        __result.push_back(__current); 
        __current++; 
    } 
    return __result; 
})()
```

### `!!>` — Auto-Decrement Until

Generates descending sequence with automatic decrement.

**Syntax:**
```lpp
start !!> limit
```

**Examples:**
```lpp
let countdown = 10 !!> 0;   // [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
let desc = 100 !!> 95;      // [100, 99, 98, 97, 96]
```

**Transpiled C++:**
```cpp
([&]() { 
    std::vector<int> __result; 
    int __current = 10; 
    int __limit = 0; 
    while (__current > __limit) { 
        __result.push_back(__current); 
        __current--; 
    } 
    return __result; 
})()
```

## Iterate-While with Custom Step

### `!! $` — Iterate While with Step Function

Most flexible variant: explicit condition and step function.

**Syntax:**
```lpp
start !! (x -> condition) $ (x -> nextValue)
```

**Examples:**
```lpp
// Even numbers
let evens = 0 !! (x -> x < 20) $ (x -> x + 2);
// [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]

// Powers of 2
let powers = 1 !! (x -> x < 100) $ (x -> x * 2);
// [1, 2, 4, 8, 16, 32, 64]

// Fibonacci (with tuple state)
let fibs = [0, 1] !! (p -> p[1] < 100) $ (p -> [p[1], p[0] + p[1]]);
// [[0,1], [1,1], [1,2], [2,3], [3,5], ..., [55,89]]

// Countdown with custom step
let by_three = 30 !! (x -> x > 0) $ (x -> x - 3);
// [30, 27, 24, 21, 18, 15, 12, 9, 6, 3]
```

**Transpiled C++:**
```cpp
([&]() { 
    std::vector<decltype(start)> __result; 
    auto __current = start; 
    while (condition(__current)) { 
        __result.push_back(__current); 
        __current = stepFn(__current); 
    } 
    return __result; 
})()
```

## Iterate-Step Alternative Syntax

### `~> !!` — Step-First Iteration

Alternative syntax that reads left-to-right: "flow into step, while condition".

**Syntax:**
```lpp
start ~> (x -> nextValue) !! (x -> condition)
```

**Examples:**
```lpp
// Powers of 2 (step-first)
let powers = 1 ~> (x -> x * 2) !! (x -> x < 100);
// [1, 2, 4, 8, 16, 32, 64]

// Factorial sequence
let factorials = 1 ~> (x -> x * 2) !! (x -> x < 10);
// [1, 2, 4, 8]

// Squares until limit
let squares = 1 ~> (x -> x + 1) !! (x -> x*x < 100);
// [1, 2, 3, 4, 5, 6, 7, 8, 9]
```

**Note:** Semantically equivalent to `!! $`, just different word order.

## Comparison with Other Languages

### Haskell
```haskell
-- Haskell
takeWhile (< 100) $ iterate (*2) 1
until (>= 100) (*2) 1
```

```lpp
-- LPP (symbolic)
1 !! (x -> x < 100) $ (x -> x * 2)
1 ~> (x -> x * 2) !! (x -> x < 100)
```

### Python
```python
# Python
result = []
x = 1
while x < 100:
    result.append(x)
    x = x * 2
```

```lpp
-- LPP (one-liner)
let result = 1 !! (x -> x < 100) $ (x -> x * 2);
```

## Combining with Other Operators

All iterate operators compose seamlessly with golf-style operators:

```lpp
// Generate, transform, filter, reduce
let result = (0 !!< 20)              // [0..19]
    @ (x -> x * x)                   // squares
    ? (x -> x % 2 == 0)              // even squares only
    \ ((acc, x) -> acc + x);         // sum
// Result: 0 + 4 + 16 + 36 + 64 + 100 + 144 + 196 + 256 + 324 = 1140

// Fibonacci sum
let fib_sum = ([0, 1] !! (p -> p[1] < 100) $ (p -> [p[1], p[0] + p[1]]))
    @ (p -> p[0])                    // extract first element
    \ ((acc, x) -> acc + x);         // sum
```

## Implementation Details

All iterate-while operators transpile to **zero-cost C++ IIFEs**:

- **No function calls** at runtime (inlined lambdas)
- **Type inference** with `decltype`
- **Efficient memory** with `std::vector::push_back`
- **Loop unrolling** opportunities for compiler

## Use Cases

### 1. Mathematical Sequences
```lpp
// Perfect squares until 1000
let squares = 1 ~> (x -> x + 1) !! (x -> x*x < 1000) @ (x -> x*x);

// Triangular numbers
let triangular = 0 ~> (x -> x + 1) !! (x -> x < 20) @ (x -> x*(x+1)/2);
```

### 2. Convergence Algorithms
```lpp
// Newton's method approximation
let approx = 1.0 !! (x -> abs(x*x - 2) > 0.001) $ (x -> (x + 2/x) / 2);
```

### 3. State Machines
```lpp
// Game of Life cell evolution
let states = initial ~> evolve !! (s -> generation < 100);
```

### 4. Numerical Methods
```lpp
// Euler's method
let trajectory = [t0, y0] ~> (p -> [p[0] + dt, p[1] + f(p)*dt]) !! (p -> p[0] < tmax);
```

## Performance Characteristics

- **Time:** O(n) where n = number of iterations
- **Space:** O(n) for result vector
- **Overhead:** Zero (inlined by compiler)

## Design Rationale

1. **Symbolic Clarity:**
   - `!!` suggests "repeat/bang repeatedly"
   - `$` suggests "apply/step"
   - `~>` suggests "flow forward"
   - `<` / `>` suggest direction

2. **Composability:** Works with `@`, `?`, `\`, pipelines

3. **Readability:** Three syntax variants for different mental models

4. **Safety:** Guaranteed termination conditions explicit in syntax

## Future Extensions

Possible additions:

- `!!!` — Infinite iteration (lazy evaluation)
- `!!?` — Iterate until predicate true (dual of !!)
- `!!#` — Iterate exactly N times (for-loop sugar)

## See Also

- [GOLF_SYNTAX.md](GOLF_SYNTAX.md) - Other symbolic operators
- [examples/test_iterate_while.lpp](../examples/test_iterate_while.lpp) - Working examples
