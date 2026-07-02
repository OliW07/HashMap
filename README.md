# hash-table

[![CI](https://github.com/OliW07/HashMap/actions/workflows/ci.yml/badge.svg)](https://github.com/OliW07/HashMap/actions)

Open-addressing hash map in development, with emphasis on cache-locality performance targeting read-heavy workloads.

## Design decisions

**Why open addressing?** No per-bucket allocations, so no pointer chasing, keeps data fully contiguous in memory

**Linear probing.** Chose it over quadratic/double hashing for simplicity and because it plays well with tombstones (all slots are interchangeable). The main downside — primary clustering — is acceptable at 70% load factor.

**Power-of-two capacity.** Enables `hash & (capacity - 1)` instead of modulo. The constructor enforces this at compile-time for `DEFAULT_CAPACITY`.

**StateSet.** Currently unused after splitting the probe helpers, but kept in the tree for when tombstone-aware operations need flexible bucket matching (e.g. skipping deleted entries during iteration, or segmenting the table). I plan to use this to implement erase operations in a clean and readable, but still optimised way.

## Not yet done

- `operator[]`
- `remove()` / `erase()` with tombstones
- Iteration
- `const`-correct iteration / `at()` safety for non-default-constructible types
- Exception safety during rehash
