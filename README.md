# hash-table

[![CI](https://github.com/OliW07/HashMap/actions/workflows/ci.yml/badge.svg)](https://github.com/OliW07/HashMap/actions)

Open-addressing hash map built as a learning exercise. Still in development.

## Design decisions

**Why open addressing?** Simpler than separate chaining — no per-bucket allocations, better cache locality.

**Linear probing.** Chose it over quadratic/double hashing for simplicity and because it plays well with tombstones (all slots are interchangeable). The main downside — primary clustering — is acceptable at 70% load factor.

**Power-of-two capacity.** Enables `hash & (capacity - 1)` instead of modulo. The constructor enforces this at runtime and at compile-time for `DEFAULT_CAPACITY`.

**Split probe helpers.** `findEntry` handles key lookup (probes past tombstones, stops at empty) and `findSlot` handles insertion (stops at first non-occupied slot). Separating them avoids branching on a `StateSet` parameter inside the probe loop.

**Rehash using a raw pointer helper.** `findSlot` takes `Bucket*` + `capacity` rather than a vector reference, so it works on both `data_` and the temporary `newData` during resize without duplicating the probe logic.

**StateSet.** Currently unused after splitting the probe helpers, but kept in the tree for when tombstone-aware operations need flexible bucket matching (e.g. skipping deleted entries during iteration, or segmenting the table).

## Not yet done

- `operator[]`
- `remove()` / `erase()` with tombstones
- Iteration
- `const`-correct iteration / `at()` safety for non-default-constructible types
- Exception safety during rehash
