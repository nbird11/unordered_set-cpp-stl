# C++ STL Unordered Set Implementation

This project implements a custom version of the C++ Standard Template Library (STL) unordered_set container. The implementation provides the same interface and performance characteristics as `std::unordered_set`.

## Overview

The custom unordered set is a hash-based container that stores unique elements. Key features include:

- Template-based implementation supporting any data type
- Customizable hash function and equality predicate
- Fast average constant-time lookup, insertion, and removal
- Automatic rehashing when load factor exceeds threshold
- Implementation using a "vector of lists" approach for buckets
- Iterator and local iterator support for traversing the container
- Customizable allocator for memory management

## Class Structure

### `custom::unordered_set<T, Hash, EqPred, A>`

The main unordered_set class template with four parameters:

- T: Type of elements
- Hash: Hash function (default `std::hash<T>`)
- EqPred: Equality predicate (default `std::equal_to<T>`)
- A: Allocator type (default `std::allocator<T>`)

### Key Constructors and Assignment

- `unordered_set()`: Default constructor
- `unordered_set(size_t numBuckets)`: Construct with specified bucket count
- `unordered_set(const unordered_set& rhs)`: Copy constructor
- `unordered_set(unordered_set&& rhs)`: Move constructor
- `unordered_set(Iterator first, Iterator last)`: Range constructor
- `operator=(const unordered_set& rhs)`: Copy assignment
- `operator=(unordered_set&& rhs)`: Move assignment
- `operator=(const std::initializer_list<T>& il)`: Initializer list assignment
- `swap(unordered_set& rhs)`: Swap contents with another set

### Iterators

- `begin()`, `end()`: Iterator access for the entire container
- `begin(size_t iBucket)`, `end(size_t iBucket)`: Local iterators for a specific bucket

### Element Access and Lookup

- `find(const T& t)`: Locate element with specific key
- `bucket(const T& t)`: Return the bucket number where element is located

### Modification

- `insert(const T& t)`: Insert element, returns pair with iterator and success bool
- `insert(const std::initializer_list<T>& il)`: Insert elements from initializer list
- `erase(const T& t)`: Remove element with specific key
- `clear()`: Remove all elements

### Capacity and Hash Policy

- `size()`: Number of elements in the container
- `empty()`: Whether the container is empty
- `bucket_count()`: Number of buckets
- `bucket_size(size_t i)`: Number of elements in a specific bucket
- `load_factor()`: Current load factor (elements/buckets)
- `max_load_factor()`: Get maximum load factor
- `max_load_factor(float m)`: Set maximum load factor
- `rehash(size_t numBuckets)`: Set number of buckets and rehash
- `reserve(size_t num)`: Reserve space for specified number of elements

## Implementation Details

The unordered_set is implemented using a "vector of lists" approach, which provides:

- Efficient hash-based access to elements
- Dynamic resizing of the bucket array when needed
- Constant average time complexity for lookups, insertions, and removals
- Preservation of element order within each bucket

The implementation maintains:

- A vector of bucket lists (`buckets`)
- The number of elements (`numElements`)
- A maximum load factor threshold (`maxLoadFactor`)
- Handles collisions through chaining in linked lists

## Usage Example

```cpp
#include "hash.h"

// Create unordered_set of integers
custom::unordered_set<int> s;

// Insert elements
s.insert(42);
s.insert(13);
s.insert(7);

// Find an element (returns iterator)
auto it = s.find(13);
if (it != s.end())
    std::cout << "Found: " << *it << std::endl;

// Check if element exists
if (s.find(99) == s.end())
    std::cout << "99 not found" << std::endl;

// Get bucket information
size_t bucket_idx = s.bucket(42);
std::cout << "Element 42 is in bucket: " << bucket_idx << std::endl;
std::cout << "Elements in that bucket: " << s.bucket_size(bucket_idx) << std::endl;

// Iterate through all elements in a specific bucket
for (auto it = s.begin(bucket_idx); it != s.end(bucket_idx); ++it) {
    std::cout << *it << " ";
}

// Iterate through all elements
for (auto it = s.begin(); it != s.end(); ++it) {
    std::cout << *it << " ";
}

// Erase an element
s.erase(7);

// Check container properties
std::cout << "Size: " << s.size() << std::endl;
std::cout << "Bucket count: " << s.bucket_count() << std::endl;
std::cout << "Load factor: " << s.load_factor() << std::endl;

// Set maximum load factor and rehash
s.max_load_factor(0.75f);
s.rehash(20);  // Request at least 20 buckets
```

## Testing

The implementation includes comprehensive unit tests in `testHash.h` that verify:

- Construction and destruction behavior with various constructors
- Assignment operators and swap functionality
- Element access and lookup operations
- Insertion and erasure functionality
- Iterator and local iterator behavior
- Bucket management and bucket-specific operations
- Load factor control and rehashing behavior
- Edge cases and corner conditions

Run tests by building in debug mode with the DEBUG flag defined.

## Files

- `hash.h`: Main unordered_set implementation
- `testHash.h`: Unit tests
- `list.h`: Custom list implementation used for buckets
- `vector.h`: Custom vector implementation used for bucket array
- Other supporting files for testing framework and dependencies

## Building

The project includes Visual Studio solution files for building on Windows. Open the solution file and build using Visual Studio 2019 or later.

## Notes

- This is an educational implementation focused on demonstrating STL container concepts
- The implementation aims to match `std::unordered_set`'s interface and performance
- The container uses lists from a custom implementation (`list.h`) and vectors from a custom implementation (`vector.h`)

## Authors

Nathan Bird  
[nathanbirdka@gmail.com](mailto:nathanbirdka@gmail.com)

Brock Hoskins  
[](.)
