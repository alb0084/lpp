# Arrow Syntax: `->` vs `=>`

L++ supports two arrow syntaxes for different purposes:

## `->` (Thin Arrow)
**Used for:** Function return types and lambda expressions with explicit type context

```lpp
// Function return type
fn add(a: int, b: int) -> int {
    return a + b;
}

// Lambda with type context (often in typed contexts)
let mapper: (int) -> int = (x -> x * 2);
let nums = [1, 2, 3] @ (x -> x * 2);
```

**Philosophy:** Traditional, explicit, familiar from Rust/TypeScript

## `=>` (Fat Arrow)  
**Used for:** Lambda expressions with implicit type inference (shorthand style)

```lpp
// Lambda with fat arrow (more functional/JS style)
let doubled = nums @ (x => x * 2);
let filtered = nums ? (x => x > 0);
let sum = nums \ ((acc, x) => acc + x);
```

**Philosophy:** Concise, functional, familiar from JavaScript/Scala

## When to Use Which?

### Use `->` when:
- Defining function return types (required)
- Lambda types need to be explicit
- Working with typed contexts
- Prefer Rust/TypeScript style

### Use `=>` when:
- Writing inline lambdas in functional chains
- Type inference is sufficient  
- Prefer JavaScript/Scala style
- Want concise, golf-style code

## Both are Equivalent for Lambdas

**Important:** For lambda expressions, `->` and `=>` are **functionally identical**. The choice is purely stylistic:

```lpp
// These are exactly the same:
let doubled1 = nums @ (x -> x * 2);
let doubled2 = nums @ (x => x * 2);

// Both transpile to the same C++ code
```

## Examples

```lpp
#pragma paradigm functional

fn process(data: [int]) -> [int] {
    return data
        ? (x => x > 0)           // Filter with =>
        @ (x => x * x)           // Map with =>
        \ ((acc, x) => acc + x); // Reduce with =>
}

fn alternative(data: [int]) -> [int] {
    return data
        ? (x -> x > 0)           // Filter with ->
        @ (x -> x * x)           // Map with ->
        \ ((acc, x) -> acc + x); // Reduce with ->
}

// Both functions are semantically identical
```

## Recommendation

**Personal/Team Consistency:** Choose one style and stick to it within a project.

**Suggested Guidelines:**
- **Use `=>` for golf-style functional code** (operators `@`, `?`, `\`)
- **Use `->` for explicit type annotations and return types**
- **Mix is allowed** but may reduce readability

```lpp
// Recommended: consistent style within chains
let result = data
    @ (x => x * 2)
    ? (x => x > 10)
    \ ((acc, x) => acc + x);

// Also valid: explicit style
let result = data
    @ (x -> x * 2)
    ? (x -> x > 10)
    \ ((acc, x) -> acc + x);
```

## Implementation Note

Both `->` and `=>` are first-class tokens in L++:
- `TokenType::ARROW` for `->`
- `TokenType::FAT_ARROW` for `=>`

The parser treats them identically for lambda expressions, giving developers stylistic freedom without semantic differences.
